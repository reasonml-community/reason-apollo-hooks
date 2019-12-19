module GetAllPersons = [%graphql
  {|
  query getAllPersons {
    allPersons {
      id
      age
      name
    }
  }
|}
];

module NewPerson = [%graphql
  {|
  subscription {
    Person {
      node {
        id
        age
        name
      }
    }
  }
|}
];

[@react.component]
let make = () => {
  let newPerson = NewPerson.make();
  let newPersonDocument = ApolloClient.gql(. newPerson##query);

  let (simple, full) = ApolloHooks.useQuery(GetAllPersons.definition);

  let subscribe = full.subscribeToMore;
  React.useEffect1(
    () => {
      Js.log("subscribe changed");
      let unsubscribe =
        subscribe(
          ~document=newPersonDocument,
          ~updateQuery=[%bs.raw
            {|
              function(prevResult, { subscriptionData }) {
                console.log(subscriptionData)
                if (!prevResult || !subscriptionData.data || !subscriptionData.data.Person)
                  return prevResult;

                return {
                  ...prevResult,
                  allPersons: prevResult.allPersons.concat(subscriptionData.data.Person.node)
                };
              }
            |}
          ],
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
