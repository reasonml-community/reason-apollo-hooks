module GetAllPersons = [%graphql
  {|
    query getAllPersons {
      allPersons {
        id
        name
      }
    }
  |}
];

// NOTE: The shape of the returned data should exactly match the shape of the GetAllPersons query
// See https://www.apollographql.com/docs/react/data/subscriptions/#subscribetomore for more information
module NewPerson = [%graphql
  {|
    subscription {
      Person {
        node {
          id
          name
        }
      }
    }
  |}
];

// Defining those types and "%identity" converters below allows us to
// write the updateQuery in pure Reason and avoid bs.raw alltogether
type person = {
  id: string,
  name: string,
};

type allPersons = {
  __typename: string,
  allPersons: array(person),
};

type subscriptionNode = {node: person};

[@bs.deriving abstract]
type newPerson = {
  [@bs.as "Person"]
  person: subscriptionNode,
};

external resultToJson: allPersons => Js.Json.t = "%identity";
external toPrevResult: Js.Json.t => Js.Nullable.t(allPersons) = "%identity";
external toSubscriptionData: Js.Json.t => Js.Nullable.t(newPerson) =
  "%identity";

[@react.component]
let make = () => {
  let newPerson = NewPerson.make();
  let newPersonDocument = ApolloClient.gql(. newPerson##query);

  let (simple, full) = ApolloHooks.useQuery(GetAllPersons.definition);

  let subscribe = full.subscribeToMore;
  React.useEffect1(
    () => {
      let unsubscribe =
        subscribe(
          ~document=newPersonDocument,
          ~updateQuery=
            (maybePrevResult, maybeSubscriptionPayload) =>
              switch (
                maybePrevResult |> toPrevResult |> Js.Nullable.toOption,
                maybeSubscriptionPayload##subscriptionData##data
                |> toSubscriptionData
                |> Js.Nullable.toOption,
              ) {
              | (Some(prev), Some(newData)) =>
                {
                  ...prev, // NOTE: This only works with BuckleScript 7
                  allPersons:
                    prev.allPersons
                    ->Belt.Array.concat([|newData->personGet.node|]),
                }
                ->resultToJson
              | _ => maybePrevResult
              },
          (),
        );

      Some(unsubscribe);
    },
    [|subscribe|],
  );

  <div className="person-list">
    {switch (simple) {
     | Loading => <p> {React.string("Loading...")} </p>
     | Data(data) =>
       data##allPersons
       ->Belt.Array.reverse
       ->Belt.Array.map(person =>
           <div key=person##id className="person">
             <div className="person-field">
               <span className="person-label"> {React.string("Id: ")} </span>
               {React.string(person##id)}
             </div>
             <div className="person-field">
               <span className="person-label">
                 {React.string("Name: ")}
               </span>
               {React.string(person##name)}
             </div>
           </div>
         )
       |> React.array
     | Error(_) => <p> {React.string("Error")} </p>
     | NoData => <p> {React.string("Not asked")} </p>
     }}
  </div>;
};
