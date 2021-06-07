module GetAllPersons = [%graphql
  {|
    query getAllPersons {
      persons {
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
      persons {
        id
        name
      }
    }
  |}
];

// Defining those types and "%identity" converters below allows us to
// write the updateQuery in pure Reason and avoid bs.raw alltogether
type person = {
  __typename: string,
  id: int,
  name: string,
};

type persons = {
  __typename: string,
  persons: array(person),
};

external resultToJson: persons => Js.Json.t = "%identity";
external toPrevResult: Js.Json.t => Js.Nullable.t(persons) = "%identity";
external toSubscriptionData: Js.Json.t => Js.Nullable.t(persons) =
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
            (maybePrevResult, maybeSubscriptionPayload) => {
              let prevResultOption =
                maybePrevResult |> toPrevResult |> Js.Nullable.toOption;
              let subscriptionDataData =
                maybeSubscriptionPayload##subscriptionData##data;
              let subscriptionDataOption =
                subscriptionDataData
                |> toSubscriptionData
                |> Js.Nullable.toOption;

              switch (prevResultOption, subscriptionDataOption) {
              | (Some(prev), Some(newData)) =>
                {
                  ...prev, // NOTE: This only works with BuckleScript 7
                  persons: newData.persons,
                }
                ->resultToJson
              | _ => maybePrevResult
              };
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
       data##persons
       ->Belt.Array.reverse
       ->Belt.Array.map(person =>
           <div key={person##id |> string_of_int} className="person">
             <div className="person-field">
               <span className="person-label"> {React.string("Id: ")} </span>
               {React.string(person##id |> string_of_int)}
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
