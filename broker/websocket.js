import jwt from 'jsonwebtoken';
import { WebSocketServer } from 'ws';
import config from './utils/config.js';
import faker from './seeders/config.js'

const CHANNELS = {
  EUR_USD: 0,
};

let lastPrice = Number(faker.finance.amount(1.1700, 1.1900, 6));
let lastQuantity = Number(faker.finance.amount(100, 1000, 4));
let lastTick = null;

const MSG = {
  USER_CONNECTION: "user connected",
  USER_DISCONNECTION: "user disconnected",
  MESSAGE_RECEPTION: "message received",
}
const defaultSymbol = 'eur_usd';
const UNAUTHORIZED = 'HTTP/1.1 401 Unauthorized';
const channels = {};
Object.values(CHANNELS).forEach((value) => {
  channels[value] = new Map();
});

function createWebSocketServer(httpServer) {
  const wss = new WebSocketServer({ clientTracking: false, noServer: true });

  wss.on('connection', async (ws, request, client, socket) => {

    ws.on('error', function(err) {
      console.log(err)
      // socket.write(`${UNAUTHORIZED}\r\n\r\n`);
      // ws.terminate();  
    });

    const url = new URL(`http://base-url/${request.url}`);
    const searchParams = url.searchParams;
    let symbol = searchParams.get('symbol');
    if (symbol === null) {
      symbol = defaultSymbol;
    }

    const currentChannel =
      CHANNELS[symbol.toUpperCase()] ||
      CHANNELS[defaultSymbol.toUpperCase()];

    channels[currentChannel].set(client.sub, { ws });
    //   let response = setUserResponse(MSG.USER_CONNECTION);
    // response = { status: "user", timestamp: Date.now()  };
    //   broadcastMessage(response, currentChannel);

    ws.on('message', (message) => {
      const rawMessage = String(message);
      onMessageReceived(ws, rawMessage, currentChannel);
    });

    ws.on('close', () => {
      const currentChannel = 0;
      const response = setUserResponse(MSG.USER_DISCONNECTION);
      broadcastMessage(response, currentChannel);
      channels[currentChannel].delete(client.sub);
      ws.terminate();
    });

  });

  httpServer.on('upgrade', function upgrade(request, socket, head) {
    authenticate(request, function next(err, client) {
      if (err || !client) {
        return destroySocket(socket, UNAUTHORIZED);
      }

      wss.handleUpgrade(request, socket, head, function done(ws) {
        wss.emit('connection', ws, request, client, socket);
      });
    });
  });
}

// ==========================================================================
//  Helpers
// ==========================================================================

function broadcastMessage(message, channelId) {
  channels[channelId].forEach((client) => {
    client.ws.send(JSON.stringify(message));
  });
}

function onMessageReceived(ws, message, channelId) {
  const order = emulateBrokerExecution(message);
  // const response = setUserResponse(MSG.MESSAGE_RECEPTION, order);
  // console.log(response)
  console.log(order)
  broadcastMessage(order, channelId);
}

function authenticate(request, next) {
  const authorization = request.headers.authorization;
  if (authorization) {
    try {
      const match = authorization.match(/^Bearer (.+)$/);
      const token = match[1];
      const payload = jwt.verify(token, config.secretKey);
      next(undefined, payload);
    } catch (error) {
      next(error);
    }
  }
}

function destroySocket(socket, status) {
  socket.write(`${status}\r\n\r\n`);
  socket.destroy();
}

function sendFakerMessage(channelId) {
  broadcastMessage(generateCoherentTicker(), channelId);
}

function setUserResponse(status, message) {
  let response = { status, timestamp: Date.now() };
  if (message) response.message = message;
  return response;
}

function generateCoherentTicker() {
  // Generate random variations with some coherence
  const priceVariation = (Math.random() * 0.005) - 0.0025; // Random change within [-0.0025, 0.0025]
  const quantityVariation = (Math.random() * 10) - 5; // Random change within [-5, 5]

  // Apply variations to last price and quantity
  lastPrice += priceVariation;
  lastQuantity += quantityVariation;

  // Ensure that price and quantity stay within reasonable bounds
  if (lastPrice < 1.1700) lastPrice = 1.1700;
  if (lastPrice > 1.1900) lastPrice = 1.1900;
  if (lastQuantity < 100) lastQuantity = 100;
  if (lastQuantity > 1000) lastQuantity = 1000;

  const date = new Date().toISOString();
  const tick = {
    type: "TICKER",
    instrument_id: "eur_usd",
    trade_id: faker.string.alphanumeric(15),
    timestamp: date,
    best_bid: (lastPrice - 0.0005).toFixed(5),
    best_ask: (lastPrice + 0.0005).toFixed(5),
    last_quantity: lastQuantity.toFixed(4),
    last_price: lastPrice.toFixed(6),
    session_open: Number(faker.finance.amount(1.1700, 1.1900, 6)), 
    session_low: Number(faker.finance.amount(1.1700, 1.1900, 6)), 
    session_high: Number(faker.finance.amount(1.1700, 1.1900, 6)) 
  };
  lastTick = tick;
  return tick;
}

setInterval(() => {
  sendFakerMessage(0);
}, 100);


function emulateBrokerExecution(str) {
  const rand = faker.number.int(1, 10)
  const BUY = 0;
  const SELL = 1;
  const order = JSON.parse(str)
  order.status = "Rejected";
  if (order.type == "Market") {

    if (order.side == BUY) {
      const price = lastTick.best_ask;
      if (rand % 10 != 0) {
        order.executed_price = price;
        order.status = "Executed";
      }
    }

    if (order.side == SELL) {
      const price = lastTick.best_bid;
      if (rand % 10 != 0) {
        order.executed_price = price;
        order.status = "Executed";
      }
    }
  }
  return order;
}


export default createWebSocketServer;

