using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Net;
using System.IO;

namespace LMDBService_ConsoleApp
{
    class Program
    {
        static void Main(string[] args)
        {
            Logger.LogInfo("Main...");
            int count = 1;
            string arg = String.Empty;
            string paramUrl = String.Empty;
            string path = String.Empty;
            if (args.Length == 3)
            {
                arg = args[0];
                if (arg == "-count")
                {
                    paramUrl = args[1];
                    count = Convert.ToInt32(args[2]);
                }
                if (arg == "-files")
                {
                    paramUrl = args[1];
                    path = args[2];
                }
                if (arg == "-post")
                {
                    paramUrl = args[1];
                }
            }
            else
            {
                Logger.LogInfo("Usage : exe ");
                Logger.LogInfo(" -count url count");
                Logger.LogInfo(" -files url path");
                return;
            }

            string str = String.Empty;

            if (arg == "-count")
            {
                str = String.Format("Count={0}", count);
                Logger.LogInfo(str);

                //string url = "http://23.100.3.100:7001/MyServer/LMDB/?request=set-data&key=key_totoB&value=value_totoB&name=cacheA2";
                string base_url = String.Format("http://{0}:7001", paramUrl);
                //string key = "Key_v0";
                //string value = "Value_v0";
                string cache = "cache_NET";
                string verb = "set-data";

                for (int i = 0; i < count; i++)
                {
                    string key = String.Format("Key_v{0}", i);
                    string value = String.Format("Value_v{0}", i);
                    string url = String.Format("{0}/MyServer/LMDB/?request={1}&key={2}&value={3}&name={4}", base_url, verb, key, value, cache);
                    Logger.LogInfo(url);

                    WGet(url);
                }

                verb = "get-data";

                for (int i = 0; i < count; i++)
                {
                    string key = String.Format("Key_v{0}", i);
                    string url = String.Format("{0}/MyServer/LMDB/?request={1}&key={2}&name={3}", base_url, verb, key, cache);
                    Logger.LogInfo(url);

                    WGet(url);
                }
            }

            if (arg == "-files")
            {
                bool bFile = false;
                if (File.Exists(path))
                    bFile = true;

                bool bDir = false;
                if (Directory.Exists(path))
                    bDir = true;

                if (bFile == true)
                {
                    StoreFile(paramUrl ,path);
                }
                else if (bDir == true)
                {
                    foreach (string file in Directory.GetFiles(path))
                    {
                        string file2 = Path.GetFileName(file);
                        //Logger.LogInfo(file);
                        //Logger.LogInfo(file2);
                        StoreFile(paramUrl, file2);
                    }
                }
            }

            if (arg == "-post")
            {
                string cache = "cache_NET";
                string base_url = String.Format("http://{0}:7001/MyServer/LMDB/?request=set-data-b64&name={1}", paramUrl, cache);

                MakeWPost(base_url);
            }
        }

        private static void MakeWPost(string url)
        {
            HttpWebRequest r = (HttpWebRequest)WebRequest.Create(url);
            string c = "\"";
            string a1 = "{";
            string a2 = "}";

            string value = String.Empty;
            for (int i = 0; i < 10000; i++)
            {
                value += "azertyuiopqsdfghjklmwxcvbn";
            }
            string valueb64 = Base64Helper.Base64Encode(value);
            string key = String.Format("key_{0}", DateTime.Now.Ticks);

            string postData = String.Format("{1}{0}key{0}:{0}{4}{0}, {0}value{0}:{0}{3}{0}{2}", c, a1, a2, valueb64, key); // url;
            //Logger.LogInfo(postData);

            var data = Encoding.ASCII.GetBytes(postData);
            r.Method = "POST";
            r.ContentType = "application/json;";
            r.ContentLength = data.Length;
            r.KeepAlive = false;

            using (var stream = r.GetRequestStream())
            {
                stream.Write(data, 0, data.Length);
            }

            WebResponse wr = r.GetResponse();
            //Logger.LogInfo(wr.ContentType);
            Stream s = wr.GetResponseStream();
            Encoding encode = System.Text.Encoding.GetEncoding("utf-8");
            StreamReader reader = new StreamReader(s, encode);
            string buffer = reader.ReadToEnd();
            Logger.LogInfo(buffer);
            reader.Close();
            wr.Close();
        }

        private static void StoreFile(string url, string path)
        {
            string cache = "cache_NET";
            string base_url = String.Format("http://{0}:7001/MyServer/LMDB/?request=set-data-b64&name={1}", url, cache);

            var enc = System.Text.Encoding.UTF8;

            string key = path;
            string value = String.Empty;
            byte[] buffer = File.ReadAllBytes(path);
            string buffer2 = enc.GetString(buffer);

            value = Base64Helper.Base64Encode(buffer2);

            MakePostBuffer(base_url, value);
        }

        private static void MakePostBuffer(string url, string valueb64)
        {
            HttpWebRequest r = (HttpWebRequest)WebRequest.Create(url);
            string c = "\"";
            string a1 = "{";
            string a2 = "}";

            string key = String.Format("key_{0}", DateTime.Now.Ticks);

            string postData = String.Format("{1}{0}key{0}:{0}{4}{0}, {0}value{0}:{0}{3}{0}{2}", c, a1, a2, valueb64, key); // url;
            //Logger.LogInfo(postData);

            var data = Encoding.ASCII.GetBytes(postData);
            r.Method = "POST";
            r.ContentType = "application/json;";
            r.ContentLength = data.Length;
            r.KeepAlive = false;

            using (var stream = r.GetRequestStream())
            {
                stream.Write(data, 0, data.Length);
            }

            string str = String.Format("Sending {0} bytes on {1}", data.Length, url);
            Logger.LogInfo(str);

            WebResponse wr = r.GetResponse();
            //Logger.LogInfo(wr.ContentType);
            Stream s = wr.GetResponseStream();
            Encoding encode = System.Text.Encoding.GetEncoding("utf-8");
            StreamReader reader = new StreamReader(s, encode);
            string buffer = reader.ReadToEnd();
            Logger.LogInfo(buffer);
            reader.Close();
            wr.Close();
        }

        private static void WGet(string url)
        {
            HttpWebRequest r = (HttpWebRequest)WebRequest.Create(url);
            r.Method = "GET";
            r.KeepAlive = false;
            WebResponse wr = r.GetResponse();
            //Logger.LogInfo(wr.ContentType);
            Stream s = wr.GetResponseStream();
            Encoding encode = System.Text.Encoding.GetEncoding("utf-8");
            StreamReader reader = new StreamReader(s, encode);
            string buffer = reader.ReadToEnd();
            Logger.LogInfo(buffer);
            reader.Close();
            wr.Close();
        }
        private static void WGet2(string url)
        {
            HttpWebRequest webReq;
            HttpWebResponse webResp = null;
            string Response = "";
            Stream reqStream = null;
            webReq = (HttpWebRequest)WebRequest.Create(url);

            Byte[] bytes = Encoding.UTF8.GetBytes("xml_doc=" + url);
            webReq.ContentType = "application/x-www-form-urlencoded";
            webReq.Method = "POST";
            webReq.ContentLength = bytes.Length;
            reqStream = webReq.GetRequestStream();
            reqStream.Write(bytes, 0, bytes.Length);
            reqStream.Close();
            webResp = (HttpWebResponse)webReq.GetResponse();

            if (webResp.StatusCode == HttpStatusCode.OK)
            {
                StreamReader loResponseStream = new StreamReader(webResp.GetResponseStream(), Encoding.UTF8);
                Response = loResponseStream.ReadToEnd();
            }

            webResp.Close();
            reqStream = null;
            webResp = null;
            webReq = null;
        }

        private static void WPost(string url)
        {
            HttpWebRequest r = (HttpWebRequest)WebRequest.Create(url);
            string postData = url;
            var data = Encoding.ASCII.GetBytes(postData);
            r.Method = "POST";
            r.ContentType = "multipart/form-data;";
            //r.ContentLength = data.Length;
            r.KeepAlive = false;

            using (var stream = r.GetRequestStream())
            {
                stream.Write(data, 0, data.Length);
            }

            WebResponse wr = r.GetResponse();
            //Logger.LogInfo(wr.ContentType);
            Stream s = wr.GetResponseStream();
            Encoding encode = System.Text.Encoding.GetEncoding("utf-8");
            StreamReader reader = new StreamReader(s, encode);
            string buffer = reader.ReadToEnd();
            Logger.LogInfo(buffer);
            reader.Close();
            wr.Close();
        }

        private static void StoreFile_GET(string paramUrl, string path)
        {
  
            var enc = System.Text.Encoding.UTF8;
            //tx.Put(db, enc.GetBytes(key), buffer);

            string key = path;
            string value = String.Empty;
            byte[] buffer = File.ReadAllBytes(path);
            string buffer2 = enc.GetString(buffer);

            value = Base64Helper.Base64Encode(buffer2);

            string verb = "set-data-b64";
            string cache = "cache_NET";
            string base_url = String.Format("http://{0}:7001", paramUrl);
            int len = buffer2.Length; 
            string url = String.Format("{0}/MyServer/LMDB/?request={1}&key={2}&value={3}&len={4}&name={5}", base_url, verb, key, value, len, cache);
            Logger.LogInfo(url);

            //WPost(url);
            WGet(url);
        }

    }

    class Base64Helper
    {
        public static string Base64Encode(string plainText)
        {
            var plainTextBytes = System.Text.Encoding.UTF8.GetBytes(plainText);
            return System.Convert.ToBase64String(plainTextBytes);
        }

        public static string Base64Decode(string base64EncodedData)
        {
            var base64EncodedBytes = System.Convert.FromBase64String(base64EncodedData);
            return System.Text.Encoding.UTF8.GetString(base64EncodedBytes);
        }
    }
}
