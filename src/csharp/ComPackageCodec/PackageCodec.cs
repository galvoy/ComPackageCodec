using ComPackageCodec.Escaper;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ComPackageCodec
{
    public class PackageCodec
    {
        public static byte[] EncodeBase64(byte version, byte[] content)
        {
            var body = EscaperBase64.Escape(EncodeGenBody(version, content));
            var pkg = new byte[body.Length + 2];
            pkg[0] = 255;
            Array.Copy(body, 0, pkg, 1, body.Length);
            pkg[pkg.Length - 1] = 254;
            return pkg;
        }

        public static byte[] EncodeConst253(byte version, byte[] content)
        {
            var body = EscaperConst253.Escape(EncodeGenBody(version, content));
            var pkg = new byte[body.Length + 2];
            pkg[0] = 255;
            Array.Copy(body, 0, pkg, 1, body.Length);
            pkg[pkg.Length - 1] = 254;
            return pkg;
        }

        private static byte[] EncodeGenBody(byte version, byte[] content)
        {
            int bodyLength = 1 + 2 + content.Length + 2;
            var body = new byte[bodyLength];

            body[0] = version; // version

            // content length
            ushort length = (ushort)content.Length;
            body[1] = (byte)(length >> 8);
            body[2] = (byte)(length & 0xFF);

            // content
            Array.Copy(content, 0, body, 3, content.Length);

            // crc
            var crc = PackageCRC.Gen(content);
            body[3 + content.Length] = crc[0];
            body[4 + content.Length] = crc[1];

            return body;
        }

        public static (byte Version, byte[] Content)? DecodeBase64(byte[] pkgBody)
        {
            var body = EscaperBase64.Unescape(pkgBody);
            return DecodeProcessBody(body);
        }

        public static (byte Version, byte[] Content)? DecodeConst253(byte[] pkgBody)
        {
            var body = EscaperConst253.Unescape(pkgBody);
            return DecodeProcessBody(body);
        }

        private static (byte Version, byte[] Content)? DecodeProcessBody(byte[]? body)
        {
            if (body == null) return null;
            if (body.Length <= 5) return null;
            var maxIndex = body.Length - 1;
            byte version = body[0];
            ushort length = (ushort)((body[1] << 8) | body[2]);
            var crcStartIndex = 3 + length;
            if (crcStartIndex + 1 > maxIndex) return null;
            var content = new byte[length];
            Array.Copy(body, 3, content, 0, length);
            var crc = PackageCRC.Gen(content);
            bool crcCorrect = body[crcStartIndex] == crc[0] && body[crcStartIndex + 1] == crc[1];
            if (!crcCorrect) return null;
            return (version, content);
        }
    }
}
