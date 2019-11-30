open ApolloHooks;

/**
 * Query response will be parsed using Config.parse from graphq_ppx before it is accessed in
 * reason, but react-apollo will save it in cache in its original format, as a regular JS object,
 * and apollo requires the data to be saved in cache in the same format or cache won't work correctly.
 *
 * If using directives like @bsRecord, @bsDecoder or @bsVariant on the query result, the data
 * in cache and the parsed data won't to have the same format. Since there is currently no way
 * to serialize the parsed data back to its initial format, queries that will be updated manually
 * in cache can't use any of those directive, unless you will take care of the serialization yourself.
 */
module PersonsNameFilterQuery = [%graphql
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

external cast: Js.Json.t => PersonsNameFilterQuery.t = "%identity";

type person = {
  .
  "age": int,
  "id": string,
  "name": string,
};

/** example using cache */
module PersonsNameFilterReadQuery =
  ApolloClient.ReadQuery(PersonsNameFilterQuery);

module PersonsNameFilterWriteQuery =
  ApolloClient.WriteQuery(PersonsNameFilterQuery);

let updateFiltered = (person: person, name, filteredPersons: array(person)) =>
  person##name === name
    ? filteredPersons->Belt.Array.concat([|person|])
    : filteredPersons->Belt.Array.keep(p => p##id !== person##id);

let updateCache = (client, person, name) => {
  let filterByNameQuery = PersonsNameFilterQuery.make(~name, ());
  let readQueryOptions = toReadQueryOptions(filterByNameQuery);

  // By default, apollo adds field __typename to the query and will use it
  // to normalize data. Parsing the result with Config.parse will remove the field,
  // which won't allow to save the data back to cache. This means we can't use ReadQuery.make,
  // which parses cache result, and have to use the readQuery which returns Json.t.
  switch (PersonsNameFilterReadQuery.readQuery(client, readQueryOptions)) {
  | exception _ => ()
  | cachedResponse =>
    switch (cachedResponse |> Js.Nullable.toOption) {
    | None => ()
    | Some(cachedPersons) =>
      // readQuery returns unparsed data as Json.t, but since PersonsNameFilterQuery
      // is not using any graphql_ppx directive, the data will have the same format,
      // (with the addition of __typename field) and can be cast to PersonsNameFilterConfig.t.
      let persons = cast(cachedPersons);
      let updatedPersons = {
        "allPersons": updateFiltered(person, name, persons##allPersons),
      };

      PersonsNameFilterWriteQuery.make(
        ~client,
        ~variables=filterByNameQuery##variables,
        ~data=updatedPersons,
        (),
      );
    }
  };
};

[@react.component]
let make = (~name) => {
  let (simple, _full) =
    useQuery(
      ~variables=PersonsNameFilterQuery.make(~name, ())##variables,
      PersonsNameFilterQuery.definition,
    );

  <div>
    {switch (simple) {
     | Loading => <p> {React.string("Loading...")} </p>
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
