
using ComPackageCodec;
using System.Text;

while (true)
{
    Console.Write("Input Content: ");
    var input = Console.ReadLine();
    if (input == null || string.IsNullOrWhiteSpace(input)) continue;
    var encoded = PackageCodec.EncodeConst253(1, Encoding.ASCII.GetBytes(input));
    Console.WriteLine($"Encoded: <{Encoding.ASCII.GetString(encoded)}>");
    var body = new Byte[encoded.Length - 2];
    Array.Copy(encoded, 1, body, 0, encoded.Length - 2);
    var decoded = PackageCodec.DecodeConst253(body);
    Console.WriteLine($"Decoded: <{Encoding.ASCII.GetString(decoded?.Content ?? [])}>");
}
