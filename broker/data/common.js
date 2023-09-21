export const ROLES_ARRAY = ['msp', 'dc','admin']
import faker from '../seeders/config.js'

export const ROLES_OBJECT = {
  MSP: ROLES_ARRAY[0],
  DC: ROLES_ARRAY[1],
  ADMIN: ROLES_ARRAY[2],
  ALL: 'all',
}

export const SITES_ARRAY = [
  ['valuruz', 'Fribourg'],
  ['vernier', 'Genève'],
  ['delémont', 'Jura'],
  ['la chaux-de-fonds', 'Neuchâtel'],
  ['sion', 'Valais'],
  ['sitten', 'Valais'],
  ['aigle', 'Vaud'],
  ['morges', 'Vaud'],
  ['pomy', 'Vaud'],
  ['renens', 'Vaud'],
]

export const SITES_OBJECT = {
  VAULRUZ: SITES_ARRAY[0][0],
  VERNIER: SITES_ARRAY[1][0],
  DELEMONT: SITES_ARRAY[2][0],
  chauxdefonds: SITES_ARRAY[3][0],
  SION: SITES_ARRAY[4][0],
  SITTEN: SITES_ARRAY[5][0],
  AIGLE: SITES_ARRAY[6][0],
  MORGES: SITES_ARRAY[7][0],
  POMY: SITES_ARRAY[8][0],
  RENENS: SITES_ARRAY[9][0],
  ALL: 'all',
}

export let sections = [
  "Horlogerie",
  "Electricité",
  "Vente",
  "Mécanique",
  "Dessin architecture",
  "Dessin industriel",
  "Informatique",
  "Support informatique",
  "Multimédia",
  "Logistique",
  "Cuisine",
  "Restauration",
  "Boulangerie",
  "Pâtisserie",
  "Horticulture",
  "Maçonnerie",
  "Peinture",
  "Paysagisme",
  "Installation sanitaire",
  "Service restauration",
  "Soins et accompagnement",
  "Carrelage",
  "Menuiserie",
  "Soudure",
  "Géomatique",
  "construction métallique",
  "Mécanique automobile",
  "Mécanique agricole",
  "Mécanique poids lourd",
]

export const SECTIONS = []
for (const site of SITES_ARRAY) {
  for (let i = 0; i < 6; i++) {
    const sectionId = faker.number.int({ min: 1, max: sections.length - 1 })
    const section = sections[sectionId]
    const siteName = site[0].charAt(0).toUpperCase() + site[0].slice(1)
    const name = `${section} - ${siteName}`
    SECTIONS.push([name, site[0]])
  }
}
