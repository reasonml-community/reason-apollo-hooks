module GetAllPersonsConfig = [%graphql
  {|
  query getAllPersons($skip: Int!, $first: Int!) {
    allPersons(skip: $skip, first: $first) {
      id
      age
      name
    }
  }
|}
];

module GetAllPersonsQuery = ReasonApolloHooks.Query.Make(GetAllPersonsConfig);

[@react.component]
let make = () => {
  let getAllPersons = GetAllPersonsConfig.make(~skip=10, ~first=10, ());
  let (simple, _full) =
    GetAllPersonsQuery.use(~variables=getAllPersons##variables, ());

  <div>
    {switch (simple) {
     | Loading => <p> {React.string("Loading...")} </p>
     | Data(data) =>
       data##allPersons
       ->Belt.Array.map(person =>
           <div key=person##id> {React.string(person##name)} </div>
         )
       |> ReasonReact.array
     | NoData
     | Error(_) => <p> {React.string("Error")} </p>
     }}
  </div>;
};
