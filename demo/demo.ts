import { SerialPort, ReadlineParser } from "serialport";

const port = new SerialPort({
  path: "/dev/ttyACM0",
  baudRate: 115200,
  // baudRate: 230400,
  // baudRate: 460800,
  // baudRate: 921600,
});

port.on("open", () => {
  console.log("serial port was open");
  port.write(`drawRectLoop 0xF800\n\n`);
  loop();
});

const parser = port.pipe(new ReadlineParser());
parser.on("data", console.log);

let i = Math.round((Math.random() * 100) % 100);
function loop() {
  port.write(`drawRect ${i} 10 50 50 0x0000\n\n`);
  // port.write(`drawRect ${i} 20 50 50 0x0000\n\n`);
  // port.write(`drawRect ${i} 30 50 50 0x0000\n\n`);
  // port.write(`drawRect ${i} 40 50 50 0x0000\n\n`);
  // port.write(`drawRect ${i} 50 50 50 0x0000\n\n`);
  // port.write(`drawRect ${i} 60 50 50 0x0000\n\n`);
  // port.write(`drawRect ${i} 70 50 50 0x0000\n\n`);
  // port.write(`drawRect ${i} 80 50 50 0x0000\n\n`);
  // port.write(`drawRect ${i} 90 50 50 0x0000\n\n`);
  if (i++ > 100) {
    console.log("reset i");
    i = 0;
  }
  port.write(`drawRect ${i} 10 50 50 0xF800\n\n`);
  // port.write(`drawRect ${i} 20 50 50 0xF800\n\n`);
  // port.write(`drawRect ${i} 30 50 50 0xF800\n\n`);
  // port.write(`drawRect ${i} 40 50 50 0xF800\n\n`);
  // port.write(`drawRect ${i} 50 50 50 0xF800\n\n`);
  // port.write(`drawRect ${i} 60 50 50 0xF800\n\n`);
  // port.write(`drawRect ${i} 70 50 50 0xF800\n\n`);
  // port.write(`drawRect ${i} 80 50 50 0xF800\n\n`);
  // port.write(`drawRect ${i} 90 50 50 0xF800\n\n`);
  setTimeout(loop, 5);
}
