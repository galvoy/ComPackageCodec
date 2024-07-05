using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ComPackageCodec.Escaper
{
    internal class EscaperBase64
    {
        public static byte[] Escape(byte[] data)
        {
            // 将字节数组转换为Base64字符串
            string base64String = Convert.ToBase64String(data);

            // 将Base64字符串转换回字节数组
            List<byte> base64Bytes = Encoding.ASCII.GetBytes(base64String).ToList();

            return base64Bytes.ToArray();
        }

        public static byte[]? Unescape(byte[] data)
        {
            try
            {
                // 将字节数组转换为Base64字符串
                string base64String = Encoding.ASCII.GetString(data);

                // 将Base64字符串转换回字节数组
                byte[] originalBytes = Convert.FromBase64String(base64String);

                return originalBytes;
            }
            catch
            {

                return null;
            }
        }
    }
}
