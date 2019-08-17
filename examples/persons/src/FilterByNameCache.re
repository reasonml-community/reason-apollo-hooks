module PersonsNameFilterConfig = [%graphql
  {|
  query getPersonsWithName($name: String!) {
    allPersons(filter: { name: $name } ) {
      id
      name
      age
    }
  }
|}
];

external cast: Js.Json.t => PersonsNameFilterConfig.t = "%identity";

type person = {
  .
  "age": int,
  "id": string,
  "name": string,
};

module PersonsNameFilterQuery =
  ReasonApolloHooks.Query.Make(PersonsNameFilterConfig);

/** example using cache */
module PersonsNameFilterReadQuery =
  ApolloClient.ReadQuery(PersonsNameFilterConfig);

module PersonsNameFilterWriteQuery =
  ApolloClient.WriteQuery(PersonsNameFilterConfig);

let updateFiltered = (person: person, name, filteredPersons: array(person)) =>
  person##name === name
    ? filteredPersons->Belt.Array.concat([|person|])
    : filteredPersons->Belt.Array.keep(p => p##id !== person##id);

let updateCache = (client, person, name) => {
  let filterByNameQuery = PersonsNameFilterConfig.make(~name, ());

  let readQueryOptions =
    ReasonApolloHooks.Utils.toReadQueryOptions(filterByNameQuery);

  switch (PersonsNameFilterReadQuery.readQuery(client, readQueryOptions)) {
  | exception _ => ()
  | cachedResponse =>
    switch (cachedResponse |> Js.Nullable.toOption) {
    | None => ()
    | Some(cachedPersons) =>
      let persons = cast(cachedPersons);
      let updatedPersons = {
        "allPersons": updateFiltered(person, name, persons##allPersons),
      };

      let mergeCacheJs: ('a, 'a) => 'a = [%bs.raw
        {| function (prev, next) {  return { ...prev, ...next }; } |}
      ];

      PersonsNameFilterWriteQuery.make(
        ~client,
        ~variables=filterByNameQuery##variables,
        ~data=mergeCacheJs(persons, updatedPersons),
        (),
      );
    }
  };
};

[@react.component]
let make = (~name) => {
  let getPersonsWithName = PersonsNameFilterConfig.make(~name, ());

  let (simple, _full) =
    PersonsNameFilterQuery.use(~variables=getPersonsWithName##variables, ());

  <div>
    {switch (simple) {
     | ReasonApolloHooks.Query.Loading =>
       <p> {React.string("Loading...")} </p>
     | Data(data) =>
       <h3>
         {"There are "
          ++ (data##allPersons->Belt.Array.length |> string_of_int)
          ++ " with name "
          ++ name
          |> React.string}
       </h3>
     | NoData
     | Error(_) => <p> {React.string("Error")} </p>
     }}
  </div>;
};
