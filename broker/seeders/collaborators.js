import faker from './config.js'
import { ROLES_ARRAY, SITES_ARRAY, ROLES_OBJECT } from '../data/common.js'
import { pickUniqRandom } from '../utils/common.js';

const MAX = 100
const PRESENCE = ['présent', 'absent', 'congé']
const SEX = ["h", "f"]

const collaborators = []
export const dashboards = []

function createDashboard(id){
  return {
    id,
    timestamp: Date.now(),
    dashboard:{}
  }
}

function getCollaboratorFactory(sex,firstName, lastName, optionalRole) {
       sex = pickUniqRandom(SEX, 1)[0]|| sex
      const randomFirstName = sex == sex[0]
        ? faker.person.firstName('male')
        : faker.person.firstName('female')

  return {
    firstName: firstName || randomFirstName,
    lastName: lastName || faker.person.lastName(),
    getCollaborator: function() {
      const id = `${this.firstName[0]}${this.firstName[1]}${this.lastName[0]}${this.lastName[1]}`.toLowerCase()
      const birthday = faker.date.past({ years: 40, refDate: "2003-01-01T00:00:00.000Z" }).getTime()
      const role = optionalRole || ROLES_ARRAY[Math.floor(Math.random() * ROLES_ARRAY.length)]
      const presence = PRESENCE[Math.floor(Math.random() * PRESENCE.length)]
      const email = `${this.firstName}.${this.lastName}@orif.ch`

      return {
        id: id,
        firstName: this.firstName,
        lastName: this.lastName,
        phone: faker.phone.number('+41 ## ### ## ##'),
        __password: id,
        email: email,
        birthday: String(birthday),
        role: [role],
        picture:this.picture,
        presence: [presence],
        site: [SITES_ARRAY[faker.number.int({ min: 0, max: SITES_ARRAY.length - 1 })][0]],
      }
    }
  }
}

for (let i = 0; i < MAX; i++) {
  collaborators.push(getCollaboratorFactory().getCollaborator())
}

const users = [
  ['h','Admin', 'Admin', ROLES_OBJECT.ADMIN]
]

users.forEach((user)=>{
collaborators.push(getCollaboratorFactory(...user).getCollaborator())
})

collaborators.forEach((collaborator)=>{
  dashboards.push(createDashboard(collaborator.id))
})

export default collaborators
