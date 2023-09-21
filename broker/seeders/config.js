import { Faker, fr, fr_CH } from '@faker-js/faker'
const faker = new Faker({
  locale: [fr_CH, fr],
});
faker.seed(Number('neos'))
export default faker
