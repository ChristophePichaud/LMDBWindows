using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Net;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using LMDBNet;
using Newtonsoft.Json;

namespace LMDBServiceNet
{
    class WebServer
    {
        HttpListener _listener;
        LMDBWrapper _wrapper;

        public WebServer(string uriPrefix)
        {
            _listener = new HttpListener();
            _listener.Prefixes.Add(uriPrefix);

            _wrapper = new LMDBWrapper();
            _wrapper.Init("cache_db");
        }
        public async void Start()
        {
            _listener.Start();
            while (true)
                try
                {
                    var context = await _listener.GetContextAsync();
                    await Task.Run(() => ProcessRequestAsync(context));
                }
                catch (HttpListenerException) { break; } // Listener stopped.
                catch (InvalidOperationException) { break; } // Listener stopped.
        }
        public void Stop() { _listener.Stop(); }

        async void ProcessRequestAsync(HttpListenerContext context)
        {
            try
            {
                string request = Path.GetFileName(context.Request.RawUrl);
                string str = String.Format("you asked for: {0}", request);
                Logger.LogInfo(str);

                Dictionary<string, string> parameters = ExtractParameters(context);

                if (parameters.Count == 0)
                {
                    await WriteResponse(context, String.Empty, HttpStatusCode.OK);
                    return;
                }

                string verb;
                if ( parameters.TryGetValue("request", out verb) == false )
                {
                    await WriteResponse(context, String.Empty, HttpStatusCode.OK);
                    return;
                }

                if (context.Request.HttpMethod == "POST")
                {
                    if (verb == Constants.VerbSetDataB64)
                    {
                        await RequestSetData64(context, parameters);
                        return;
                    }
                    else if (verb == Constants.VerbGetDataB64)
                    {
                        await RequestGetData64(context, parameters);
                        return;
                    }
                }

                if (context.Request.HttpMethod == "GET")
                {
                    if (verb == Constants.VerbPing)
                    {
                        await RequestVerbPing(context);
                        return;
                    }
                    else if (verb == Constants.VerbUsage)
                    {
                        await RequestUsage(context);
                        return;
                    }
                    else if (verb == Constants.VerbGetData)
                    {
                        await RequestVerbGetData(context, parameters);
                        return;
                    }
                    else if (verb == Constants.VerbSetData)
                    {
                        await RequestVerbSetData(context, parameters);
                        return;
                    }
                    else if (verb == Constants.VerbGetDataB64)
                    {
                        await RequestGetData64(context, parameters);
                        return;
                    }
                }

                await WriteResponse(context, String.Empty, HttpStatusCode.OK);
            }
            catch (Exception ex) { Console.WriteLine("Request error: " + ex); }
        }

        private async Task RequestGetData64(HttpListenerContext context, Dictionary<string, string> parameters)
        {
            string key;
            string value;

            if (parameters.TryGetValue("key", out key) == false)
            {
                await WriteResponse(context, String.Empty, HttpStatusCode.OK);
                return;
            }

            _wrapper.GetData(key, out value);

            Data data = new Data();
            data.Key = key;
            data.Value = Base64Helper.Base64Decode(value);

            context.Response.AddHeader("Content-disposition", "attachment; filename=" + key);
            context.Response.ContentType = "application/octet-stream";
            context.Response.ContentLength64 = data.Value.Length;

            Encoding encode = System.Text.Encoding.GetEncoding("utf-8");
            Stream s = context.Response.OutputStream;
            byte[] buffer = encode.GetBytes(data.Value);
            s.Write(buffer, 0, data.Value.Length);
            s.Close();

            return;

            //string str = JsonConvert.SerializeObject(data);
            //await WriteResponse(context, key, HttpStatusCode.OK);
        }

        private async Task RequestSetData64(HttpListenerContext context, Dictionary<string, string> parameters)
        {
            //string content = context.Request.ContentType;
            Encoding encode = System.Text.Encoding.GetEncoding("utf-8");
            Stream s = context.Request.InputStream;
            StreamReader reader = new StreamReader(s, encode);
            string buffer = reader.ReadToEnd();
            //Logger.LogInfo(buffer);
            reader.Close();

            long len = context.Request.ContentLength64;
            string str = String.Format("POST Data Length={0}", len);
            Logger.LogInfo(str);

                                                                                                                                                                                                            Data data = JsonConvert.DeserializeObject<Data>(buffer);
            string data20 = data.Value.Substring(0, data.Value.Length > 20 ? 20 : data.Value.Length);
            str = String.Format("Key:{0} Value:{1}...", data.Key, data20); // data.Value);
            Logger.LogInfo(str);

            _wrapper.SetData(data.Key, data.Value);

            //string value = Base64Helper.Base64Decode(data.Value);
            //str = String.Format("Decoded64 Value:{0}", value);
            //Logger.LogInfo(str);

            await WriteResponse(context, String.Empty, HttpStatusCode.OK);
        }

        private Dictionary<string, string> ExtractParameters(HttpListenerContext context)
        {
            string request = Path.GetFileName(context.Request.RawUrl);
            string str = String.Format("Request: {0}", request);
            //Console.WriteLine(str);

            //http://192.168.175.241:7001/MyServer/LMDB/?request=set-data&key=toto0&value=toto1&name=cache2

            Dictionary<string, string> parameters = new Dictionary<string, string>();

            if (String.IsNullOrEmpty(request))
                return parameters;

            var strings = request.Split('&');

            if (strings == null)
                return parameters;

            if (strings.Length >= 1)
            {
                strings[0] = strings[0].Remove(0, 1);
            }
            else
            {
                return parameters;
            }

            foreach (string s in strings)
            {
                //Console.WriteLine(s);
                var data = s.Split('=');
                if( data.Length == 2)
                {
                    string values = String.Format("{0}:{1}", data[0], data[1]);
                    //Console.WriteLine(values);

                    parameters.Add(data[0], data[1]);

                }
            }

            return parameters;

        }

        private async Task WriteResponse(HttpListenerContext context, string message, HttpStatusCode code)
        {
            Logger.LogInfo(message);

            var enc = Encoding.UTF8;
            byte[] msg = enc.GetBytes(message);

            context.Response.StatusCode = (int)code;

            context.Response.ContentLength64 = msg.Length;
            using (Stream s = context.Response.OutputStream)
                await s.WriteAsync(msg, 0, msg.Length);
        }

        private async Task RequestUsage(HttpListenerContext context)
        {

            DataUsage usage = new DataUsage();
            usage.company = "NEOS-SDI";
            usage.developer = "Christophe Pichaud";
            usage.version = "August 2008 BETA 0.3 .NET C#";

            string str = JsonConvert.SerializeObject(usage);

            await WriteResponse(context, str, HttpStatusCode.OK);
        }

        private async Task RequestVerbPing(HttpListenerContext context)
        {
            DataPing ping = new DataPing();
            ping.ip = "localhost";
            ping.port = Constants.MasterNodePort;
            ping.server = Environment.MachineName;

            string str = JsonConvert.SerializeObject(ping);

            await WriteResponse(context, str, HttpStatusCode.OK);
        }

        private async Task RequestVerbSetData(HttpListenerContext context, Dictionary<string, string> parameters)
        {
            string key;
            string value;

            if (parameters.TryGetValue("key", out key) == false ||
                parameters.TryGetValue("value", out value) == false)
            {
                await WriteResponse(context, String.Empty, HttpStatusCode.OK);
                return;
            }

            _wrapper.SetData(key, value);

            Data data = new Data();
            data.Key = key;
            data.Value = value;

            string str = JsonConvert.SerializeObject(data);
            await WriteResponse(context, str, HttpStatusCode.OK);
        }

        private async Task RequestVerbGetData(HttpListenerContext context, Dictionary<string, string> parameters)
        {
            string key;
            string value;

            if (parameters.TryGetValue("key", out key) == false)
            {
                await WriteResponse(context, String.Empty, HttpStatusCode.OK);
                return;
            }

            _wrapper.GetData(key, out value);

            Data data = new Data();
            data.Key = key;
            data.Value = value;

            string str = JsonConvert.SerializeObject(data);
            await WriteResponse(context, str, HttpStatusCode.OK);
        }
    }
}
