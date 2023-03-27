# huffman-code-archiver
Simple archiver based on Huffman coding

## Usage
`make` to compile

`./huffman [flags]` to run

`-c` - to compress

`-u` - to decompress

`-f <path>` / `--file <path>` - input file name

`-o <path>` / `--output <path>` - output file name

## Sample output

`$ ./huffman -c -f myfile.txt -o result.bin`

`15678` -- Size of source data

`6172` -- Size of encoded data (without additional information)

`482` -- Size of additional information


`$ ./huffman -u -f result.bin -o myfile_new.txt`

`6172` -- Size of encoded data (without additional information)

`15678` -- Size of decoded data

`482` -- Size of additional information
