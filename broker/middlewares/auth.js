import MSG from '../utils/msg.js'
import { send } from '../utils/common.js'
import useAuth from '../utils/useAuth.js'
import config from '../utils/config.js'

  const auth = async (req, res, next) => {
    if (req.url === `/${config.apiName}/auth`) return next()
    const authorization = req.get('Authorization');
    if (!authorization)
      return send(res, { error: MSG.ERROR_HEADER_REQUIRED }, 401)
    const match = authorization.match(/^Bearer (.+)$/);
    if (!match)
      return send(res, { error: MSG.ERROR_TOKEN_FORMAT }, 401)
    const token = match[1];
    const payload = useAuth.verifyJwtToken(token)
    if (!payload)
      return send(res, { error: MSG.ERROR_TOKEN_VALIDATION }, 401)
    next()
  }
export default auth
