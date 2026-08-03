## Basic Exercise 3 - Simple Shell - 25%
Once UART is configured correctly, you can build a simple shell interface to enable basic interaction between the OrangePi RV2 and your host computer.

The shell should process user input received via UART and respond with predefined messages. At minimum, it must support the following commands:

 - help: Display a list of available commands.
 - hello: Display the message “Hello World!”

You may implement a simple command parser that reads input character by character, identifies complete commands, and prints the corresponding output.

The expected result is shown below:

Expected result of the simple shell
Important

Be mindful of character alignment issues when handling screen I/O. Consider translating newline characters (\n) into carriage return + newline (\r\n) to ensure proper display across different serial terminals.

```
Todo

Implement a basic shell that reads input from UART and displays output accordingly. The shell should recognize the listed commands and print appropriate responses.
```