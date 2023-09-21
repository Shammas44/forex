import fs from 'fs';
import jsonServer from "json-server";
import config from './utils/config.js';
import data from './db.js';
import { removePrivateField } from './utils/common.js';
import { authRoute } from './routes/auth.js';
import cors from 'cors';
import authMiddleware from './middlewares/auth.js';
import https from 'https';
import createWebSocketServer from './websocket.js';

const corsOptions = {
  origin: ['http://localhost:4200', 'https://neos.sebastientraber.com'],
  exposedHeaders: ['data-timestamp'],
  methods: "*",
};
const apiPrefix = `/${config.apiName}`;

const server = jsonServer.create();
const router = jsonServer.router(data);
const middlewares = jsonServer.defaults();

const privateKey = fs.readFileSync('key.pem', 'utf8');
const certificate = fs.readFileSync('cert.pem', 'utf8');
const credentials = { key: privateKey, cert: certificate };

server.use(cors(corsOptions));
server.use(jsonServer.bodyParser);

server.post(`${apiPrefix}/auth`, authRoute(router));

router.render = (req, res) => {
  res.set('data-timestamp', config.date);
  let data = res.locals.data;
  data = removePrivateField(data);
  res.jsonp(data);
};

server.use(middlewares);
server.use(authMiddleware);
server.use(apiPrefix, router);

// Create HTTPS server
const httpsServer = https.createServer(credentials, server);

// Start the server
const port = config.port;
httpsServer.listen(port, () => {
  console.log(`JSON Server with HTTPS is running on port ${port}`);
});

createWebSocketServer(httpsServer);
