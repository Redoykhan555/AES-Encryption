This is a simple implementation of AES encryption algorithm for learning purpose, and is not intended for professional use.

Both encrypt.exe and decrypt.exe take 3 parameters:
  <ul>
  <li>-k : full path for key file. "key.k" is default.</li>
  <li>-i : full path for input file. "input.txt" & "encrypted.enc" is respectively default for encryption and decryption.</li>
  <li>-o : full path for output file. "encrypted.enc" & "out.txt" is respectively default for encryption and decryption.</li>
 </ul>

Acknowledgement : This program directly uses <a href="https://en.wikipedia.org/wiki/Rijndael_MixColumns">this wikipedia code</a> for 
byte multiplication in finite field.
