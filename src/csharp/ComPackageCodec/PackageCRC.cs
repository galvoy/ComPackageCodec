namespace ComPackageCodec
{
    internal class PackageCRC
    {
        private const ushort polynomial = 0x1021;
        private static readonly ushort[] table = ComputeCRCTable(polynomial);

        private static ushort[] ComputeCRCTable(ushort crcPolynomial)
        {
            var crcTable = new ushort[256];
            ushort value;
            ushort temp;
            for (ushort i = 0; i < crcTable.Length; ++i)
            {
                value = 0;
                temp = i;
                for (byte j = 0; j < 8; ++j)
                {
                    if (((value ^ temp) & 0x0001) != 0)
                    {
                        value = (ushort)(value >> 1 ^ crcPolynomial);
                    }
                    else
                    {
                        value >>= 1;
                    }
                    temp >>= 1;
                }
                crcTable[i] = value;
            }
            return crcTable;
        }

        public static byte[] Gen(byte[] bytes)
        {
            ushort crcValue = 0xffff;

            for (int i = 0; i < bytes.Length; ++i)
            {
                byte index = (byte)((crcValue >> 8 ^ bytes[i]) & 0xff);
                crcValue = (ushort)(crcValue << 8 ^ table[index]);
            }
            return BitConverter.GetBytes(crcValue);
        }
    }

}
