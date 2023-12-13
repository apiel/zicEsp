var net = require("net");

async function sleep(ms: number) {
  return new Promise((resolve) => {
    setTimeout(resolve, ms);
  })
}

var client = new net.Socket();
client.connect(5555, "192.168.1.107", async () => {
  console.log("Connected");
  client.write('drawRect 10 10 100 100 0x07FF \n');
  await sleep(1000);
  client.write('drawRectLoop 0xF800\n');
  // client.write("Hello, server! Love, Client.");
  await sleep(1000);
  loop();
});

let i = Math.round((Math.random() * 100) % 100);
function loop() {
  // client.write(`drawRect ${i} 10 50 50 0x0000\n`);
  // port.write(`drawRect ${i} 20 50 50 0x0000\n`);
  // port.write(`drawRect ${i} 30 50 50 0x0000\n`);
  // port.write(`drawRect ${i} 40 50 50 0x0000\n`);
  // port.write(`drawRect ${i} 50 50 50 0x0000\n`);
  // port.write(`drawRect ${i} 60 50 50 0x0000\n`);
  // port.write(`drawRect ${i} 70 50 50 0x0000\n`);
  // port.write(`drawRect ${i} 80 50 50 0x0000\n`);
  // port.write(`drawRect ${i} 90 50 50 0x0000\n`);
  if (i++ > 100) {
    console.log("reset i");
    i = 0;
  }
  client.write(`drawRect ${i} 10 50 50 0xF800\n`);
  // port.write(`drawRect ${i} 20 50 50 0xF800\n`);
  // port.write(`drawRect ${i} 30 50 50 0xF800\n`);
  // port.write(`drawRect ${i} 40 50 50 0xF800\n`);
  // port.write(`drawRect ${i} 50 50 50 0xF800\n`);
  // port.write(`drawRect ${i} 60 50 50 0xF800\n`);
  // port.write(`drawRect ${i} 70 50 50 0xF800\n`);
  // port.write(`drawRect ${i} 80 50 50 0xF800\n`);
  // port.write(`drawRect ${i} 90 50 50 0xF800\n`);
  setTimeout(loop, 1000);
}
