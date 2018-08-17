using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Net;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace LMDBServiceNet
{
    class WebServer
    {
        HttpListener _listener;

        public WebServer(string uriPrefix)
        {
            _listener = new HttpListener();
            _listener.Prefixes.Add(uriPrefix);
        }
        public async void Start()
        {
            _listener.Start();
            while (true)
                try
                {
                    var context = await _listener.GetContextAsync();
                    Task.Run(() => ProcessRequestAsync(context));
                }
                catch (HttpListenerException) { break; } // Listener stopped.
                catch (InvalidOperationException) { break; } // Listener stopped.
        }
        public void Stop() { _listener.Stop(); }

        async void ProcessRequestAsync(HttpListenerContext context)
        {
            try
            {
                string verb = Path.GetFileName(context.Request.RawUrl);

                byte[] msg;
                string str = String.Format("you asked for: {0}", verb);
                var enc = Encoding.UTF8;

                context.Response.StatusCode = (int)HttpStatusCode.OK;
                msg = enc.GetBytes(str);

                context.Response.ContentLength64 = msg.Length;
                using (Stream s = context.Response.OutputStream)
                    await s.WriteAsync(msg, 0, msg.Length);
            }
            catch (Exception ex) { Console.WriteLine("Request error: " + ex); }
        }
    }
}
