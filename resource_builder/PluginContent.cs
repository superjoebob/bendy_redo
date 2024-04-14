using System;
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
        };
    }
}