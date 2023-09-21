import jwt  from 'jsonwebtoken'
import config  from './config.js'

const roles = {
  admin: 'admin',
  user: 'user',
};

  const useAuth = {
    generateJwtToken(userId, isAdmin, numberOfDay = 7) {
      const expirationDate = Math.floor(Date.now() / 1000) + numberOfDay * 24 * 3600;
      const role = isAdmin ? roles.admin : roles.user;
      const payload = { sub: userId, exp: expirationDate, scope: role };
      try {
        return { token: jwt.sign(payload, config.secretKey) };
      } catch (error) {
        return null
      }
    },
    verifyJwtToken(token) {
      try {
        return { payload: jwt.verify(token, config.secretKey) };
      } catch (error) {
        return null
      }
    },
    getPayloadFromToken(req) {
      const authorization = req.get('Authorization');
      if (!authorization) return {};
      const match = authorization.match(/^Bearer (.+)$/);
      if (!match) return {};
      const token = match[1];
      try {
        const payload = jwt.verify(token, config.secretKey);
        return payload;
      } catch (error) {
        return {};
      }
    },
  };

export default useAuth
