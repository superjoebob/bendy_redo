using System;
using System.Collections.Generic;
using System.Drawing;
using System.Drawing.Imaging;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

class Program
{
    enum ContentIndex
    {
        Texture,
        Sound,
        Shader,
        Font,
        Other,

        All,
        Cache = All
    }

    static DirectoryInfo _inputDir;
    static DirectoryInfo _outputDir;
    static FileInfo _outputMetadataFile;
    static Queue<Tuple<ContentIndex, FileInfo>> _files = new Queue<Tuple<ContentIndex, FileInfo>>();
    static void Main(string[] pCommandLine)
    {
        float scale = 1.0f;
        for (int i = 0; i < pCommandLine.Length; i++)
        {
            if (pCommandLine[i] == "-out")
            {
                i += 1;

                pCommandLine[i] = pCommandLine[i].Replace("\"", "");
                pCommandLine[i] = pCommandLine[i].Replace("\n", "");
                pCommandLine[i] = pCommandLine[i].Replace("\r", "");
                if (pCommandLine[i].EndsWith("\\"))
                    pCommandLine[i] = pCommandLine[i].Substring(0, pCommandLine[i].Length - 1);

                Console.WriteLine("Setting output directory: " + pCommandLine[i]);
                _outputDir = new DirectoryInfo(pCommandLine[i]);
            }
            else if (pCommandLine[i] == "-in")
            {
                i += 1;

                pCommandLine[i] = pCommandLine[i].Replace("\"", "");
                pCommandLine[i] = pCommandLine[i].Replace("\n", "");
                pCommandLine[i] = pCommandLine[i].Replace("\r", "");

                Console.WriteLine("Setting input directory: " + pCommandLine[i]);
                _inputDir = new DirectoryInfo(pCommandLine[i]);
            }
            else if (pCommandLine[i] == "-metadata")
            {
                i += 1;

                pCommandLine[i] = pCommandLine[i].Replace("\"", "");
                pCommandLine[i] = pCommandLine[i].Replace("\n", "");
                pCommandLine[i] = pCommandLine[i].Replace("\r", "");

                Console.WriteLine("Setting metadata file: " + pCommandLine[i]);
                _outputMetadataFile = new FileInfo(pCommandLine[i]);
            }
            else if (pCommandLine[i] == "-scale")
            {
                i += 1;
                scale = Convert.ToInt32(pCommandLine[i]);
            }
        }

        var entries = from a in _inputDir.EnumerateFileSystemInfos("*", SearchOption.AllDirectories)
                      orderby a.Attributes.HasFlag(FileAttributes.Directory) descending, a.FullName ascending
                      select a;


        Console.WriteLine($"Found { entries.Count() } resource files.");
        foreach (var info in entries)
        {
            if (info.Attributes.HasFlag(FileAttributes.Directory))
                _outputDir.CreateSubdirectory(info.FullName.Substring(_inputDir.FullName.Length + 1));
            else
            {
                ContentIndex contentType;

                switch (info.Extension)
                {
                    case ".png":
                        contentType = ContentIndex.Texture;
                        break;

                    default:
                        contentType = ContentIndex.Other;
                        break;
                }

                _files.Enqueue(Tuple.Create(contentType, (FileInfo)info));
            }
        }

        string outputH = "";

        string outputFile =

@"# include ""resource.h""
#ifndef VST_RESOURCEDATA_LINKS
";

        string metadataFile =
@"using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace synthman
{
    public class PluginContent : IContentDictionary
    {
        public Resource Get(string pName)
        {
            Resource res = null;
            _data.TryGetValue(pName, out res);
            return res;
        }

        private Dictionary<string, Resource> _data = new Dictionary<string, Resource>()
        {
";



int idx = 101;
        while (true)
        {
            if (_files.Count == 0)
                break;

            Tuple<ContentIndex, FileInfo> entry = _files.Dequeue();


            ContentIndex x = entry.Item1;
            FileInfo info = entry.Item2;

            var relative = info.FullName.Substring(_inputDir.FullName.Length + 1);
            var relativeFile = _outputDir.FullName + "\\" + relative.Substring(0, relative.Length - Path.GetExtension(relative).Length);


            try
            {
                switch (x)
                {
                    case ContentIndex.Texture:

                        float realScale = scale;
                        if (Path.GetFileNameWithoutExtension(info.FullName).EndsWith("_halfres"))
                            realScale /= 2;

                        Bitmap image = new Bitmap(info.FullName);
                        Bitmap result = new Bitmap((int)(image.Width * realScale), (int)(image.Height * realScale));
                        using (Graphics g = Graphics.FromImage(result))
                        {
                            g.SmoothingMode = System.Drawing.Drawing2D.SmoothingMode.None;
                            g.InterpolationMode = System.Drawing.Drawing2D.InterpolationMode.NearestNeighbor;
                            g.PixelOffsetMode = System.Drawing.Drawing2D.PixelOffsetMode.Half;
                            g.DrawImage(image, 0, 0, image.Width * realScale, image.Height * realScale);
                        }

                        //Replace pink (255, 0, 255) with transparency
                        var bits = result.LockBits(new Rectangle(0, 0, result.Width, result.Height), ImageLockMode.ReadOnly, PixelFormat.Format32bppArgb);
                        unsafe
                        {
                            var wh = result.Width * result.Height;
                            byte* inData = (byte*)bits.Scan0.ToPointer();
                            byte* inDataPtrMovable = inData;
                            for (int bt = 0; bt < wh; ++bt)
                            {
                                if (inData[0] == 255 && inData[1] == 0 && inData[2] == 255 && inData[3] == 255)
                                {
                                    for (var i = 0; i < 4; ++i)
                                        *inDataPtrMovable++ = 0;
                                }
                                else
                                {
                                    for (var i = 0; i < 4; ++i)
                                        inDataPtrMovable++;
                                }

                                inData += 4;
                            }
                        }
                        result.UnlockBits(bits);


                        string saveDir = _outputDir.FullName + "\\assets\\" + relative;
                        Directory.CreateDirectory(Path.GetDirectoryName(saveDir));
                        result.Save(saveDir);
                        
                        string shortName = info.FullName.Replace(_inputDir.FullName + "\\", "").Replace(".png", "");
                        string name = "PNG_" + shortName;
                        //outputFile += name + " PNG " + "\"" + info.FullName.Replace("\\", "\\\\") + "\"\n";
                        outputFile += name + " PNG " + "\"" + saveDir.Replace("\\", "\\\\") + "\"\n";
                        outputH += "#define " + name + " " + idx.ToString() + "\n";
                        metadataFile += "           { \"" + shortName + "\", new Resource() { name = \"" + shortName + "\", id = " + idx.ToString() + ", size = new Point(" + image.Width + ", " + image.Height + "), scale = " + realScale + "f } },\n";
                        idx++;
                        break;
                }
            }
            catch (Exception e)
            {
                Console.WriteLine((info as FileInfo).FullName.Replace(_inputDir.FullName, "") + " failed to build properly: " + e.Message);
            }
        }

        outputFile +=
@"#endif";

        outputH +=
@"# ifdef APSTUDIO_INVOKED
# ifndef APSTUDIO_READONLY_SYMBOLS
#define _APS_NEXT_RESOURCE_VALUE        103
#define _APS_NEXT_COMMAND_VALUE         40001
#define _APS_NEXT_CONTROL_VALUE         1001
#define _APS_NEXT_SYMED_VALUE           101
#endif
#endif
";

        metadataFile +=
@"        };
    }
}";

        File.WriteAllText(_inputDir.FullName + "/../Resource.rc", outputFile);
        File.WriteAllText(_inputDir.FullName + "/../resource.h", outputH);
        //File.WriteAllText(_outputMetadataFile.FullName, metadataFile);

    }
}