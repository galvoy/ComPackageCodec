using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ComPackageCodec.Escaper
{
    internal class EscaperConst253
    {
        // 转义字符：253
        // 转义表：
        // 253 => 253 0
        // 254 => 253 1
        // 255 => 253 2
        // 反转义缺省值：253 X => X

        public static byte[] Escape(byte[] data)
        {
            List<byte> converted = [];
            for (int index = 0; index < data.Length; index++)
            {
                var value = data[index];
                if (data[index] == 253) // 253 => 253 0
                {
                    converted.Add(253);
                    converted.Add(0);
                }
                else if (data[index] == 254) // 254 => 253 1
                {
                    converted.Add(253);
                    converted.Add(1);
                }
                else if (data[index] == 255) // 255 => 253 2
                {
                    converted.Add(253);
                    converted.Add(2);
                }
                else // 无需转义
                {
                    converted.Add(value);
                }
            }
            return converted.ToArray();
        }

        public static byte[] Unescape(byte[] data)
        {
            List<byte> converted = [];
            var maxIndex = data.Length - 1;
            for (int index = 0; index < data.Length; index++)
            {
                var value = data[index];
                if (value != 253) // 非转义符
                {
                    converted.Add(value);
                    continue;
                }
                // 转义符，进入反转义
                if (index + 1 > maxIndex) break;
                var valueNext = data[index + 1];
                index += 1; // index + 1 已被处理，需要跳过
                if (valueNext == 0) // 253 0 => 253
                {
                    converted.Add(253);
                }
                else if (valueNext == 1) // 253 1 => 254
                {
                    converted.Add(254);
                }
                else if (valueNext == 2) // 253 2 => 255
                {
                    converted.Add(255);
                }
                else // 反转义缺省值：253 X => X
                {
                    converted.Add(valueNext);
                }
            }
            return converted.ToArray();
        }
    }
}
