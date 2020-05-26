// example of creating a fragment on a type available from your grapqlql-ppx types.

// In this contrived example, here instead of using the following as seen in the `FilterByAge.re`
// module PersonsOlderThanQuery = [%graphql
//   {|
//   query getPersonsOlderThan($age: Int!) {
//     allPersons(filter: { age_gte: $age } ) {
//       id
//     }
//   }
// |}
// ];

// we can use the following as seen in `FilterByAgeFragment.re`
// ```
// module PersonsOlderThanQuery = [%graphql
//   {|
//   query getPersonsOlderThan($age: Int!) {
//     allPersons(filter: { age_gte: $age } ) {
//       ...PersonFragment.Person
//     }
//   }
// |}
// ];
// ```
module PersonFragment = [%graphql
  {|
  fragment person on Person {
      id
      name
      age
  }
|}
];
module PersonIdFragment = [%graphql
  {|
  fragment person on Person {
      id
  }
|}
];
module PersonAgeFragment = [%graphql
  {|
  fragment person on Person {
      age
  }
|}
];