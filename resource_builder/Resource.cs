using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace synthman
{
    public struct Point
    {
        public Point(float xpos, float ypos)
        {
            x = xpos;
            y = ypos;
        }
        public float x;
        public float y;

        public static readonly Point Zero = new Point(0, 0);
        public static readonly Point MinValue = new Point(int.MinValue, int.MinValue);
        public static readonly Point MaxValue = new Point(int.MaxValue, int.MaxValue);

        override public string ToString()
        {
            return "x: " + x + " y: " + y;
        }

        public static float DistScore(Point p1, Point p2, int max, int range = 100, bool inverse = false)
        {
            float d = Math.Min(Dist(p1, p2), max);
            float f = d / (float)max;
            if (inverse)
                f = 1.0f - f;

            return f * range;
        }

        public static float DistScore(float d, int max, int range = 100, bool inverse = false)
        {
            d = Math.Min(d, max);
            float f = d / (float)max;
            if (inverse)
                f = 1.0f - f;

            return f * range;
        }

        public static float Dist(Point p1, Point p2)
        {
            Point dif = p1 - p2;
            return Math.Abs(dif.x) + Math.Abs(dif.y);
        }

        public static Point operator -(Point value)
        {
            value.x = -value.x;
            value.y = -value.y;
            return value;
        }

        public static bool operator ==(Point value1, Point value2)
        {
            return value1.x == value2.x && value1.y == value2.y;
        }

        public static bool operator !=(Point value1, Point value2)
        {
            return value1.x != value2.x || value1.y != value2.y;
        }

        public static Point operator +(Point value1, Point value2)
        {
            value1.x += value2.x;
            value1.y += value2.y;
            return value1;
        }

        public static Point operator -(Point value1, Point value2)
        {
            value1.x -= value2.x;
            value1.y -= value2.y;
            return value1;
        }

        public static Point operator *(Point value1, Point value2)
        {
            value1.x *= value2.x;
            value1.y *= value2.y;
            return value1;
        }

        public static Point operator /(Point value1, float value2)
        {
            value1.x = (int)(value1.x / value2);
            value1.y = (int)(value1.x / value2);
            return value1;
        }
    }

    public interface IContentDictionary
    {
        Resource Get(string pName);
    }
    public static partial class Content
    {
        private static IContentDictionary _content;
        public static void SetContent(IContentDictionary pContent) { _content = pContent; }
        public static Resource Get(string pName, bool pCanReturnNull = false)
        {
            if (_content == null)
            {
                if (pCanReturnNull)
                    return null;

                throw new Exception("Content.Get() failed: Content.SetContent(new PluginContent()) was never called.");
            }
            Resource r = _content.Get(pName);

            if (r == null && !pCanReturnNull)
                throw new Exception("Content.Get() failed: Could not find resource named \"" + pName + "\".");

            return r;
        }
    }

    public class Resource
    {
        public string name;
        public int id;
        public Point size;
        public float scale;
        public int width { get { return (int)size.x; } }
        public int height { get { return (int)size.y; } }

        public IntPtr platform;
    }
}