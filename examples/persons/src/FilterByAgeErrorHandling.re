// To Test this component, turn off your internet connection or disable wifi to see the error message produced in the browser

open ApolloHooks;

module PersonsOlderThanQuery = [%graphql
  {|
  query getPersonsOlderThan($age: Int!) {
    allPersons(filter: { age_gte: $age } ) {
      id
    }
  }
|}
];

[@react.component]
let make = (~age) => {
  let (simple, _full) =
    useQuery(
      ~errorPolicy=All,
      ~variables=PersonsOlderThanQuery.makeVariables(~age, ()),
      PersonsOlderThanQuery.definition,
    );

  <div>
    {switch (simple) {
     | Loading => <p> {React.string("Loading...")} </p>
     | Data(data) =>
       <h3>
         {"There are "
          ++ (data##allPersons->Belt.Array.length |> string_of_int)
          ++ " people older than "
          ++ string_of_int(age)
          |> React.string}
       </h3>
     | NoData => React.null
     | Error(error) => <p> {React.string(error##message)} </p>
     }}
  </div>;
};