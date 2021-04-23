# x_to_json
Json parsing/construction library written in c++

## Documentation
[Access the documentation here](https://github.com/Cattimus/x_to_json/wiki)

## Sidenote and warnings
This project was originally created as an exercise in writing your own portable library and using it in another project. I chose json specifically because it's required for virtually everything on the web and does not have native support in most languages. 

That being said, using this code in production is likely a very bad idea. While I've done my best to prevent harmful mistakes, I haven't covered every possible case or every possible error. You shouldn't be using this library in any situation where you may face any kind of security threats, since it will likely be trivial to exploit.
