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

let personsPerPage = 10;

[@react.component]
let make = () => {
  let skipRef = React.useRef(0);
  let getAllPersons =
    GetAllPersonsConfig.make(~skip=0, ~first=personsPerPage, ());

  let (simple, full) =
    GetAllPersonsQuery.use(
      ~variables=getAllPersons##variables,
      ~notifyOnNetworkStatusChange=true,
      (),
    );

  let handleLoadMore = _ => {
    let skip = React.Ref.current(skipRef) + personsPerPage;
    let getAllPersons2 =
      GetAllPersonsConfig.make(~skip, ~first=personsPerPage, ());

    full.fetchMore(
      ~variables=getAllPersons2##variables,
      ~updateQuery=[%bs.raw
        {|
          function(prevResult, { fetchMoreResult, ...rest }) {
            if (!fetchMoreResult) return prevResult;
            return {
                ...fetchMoreResult,
                allPersons: prevResult.allPersons.concat(fetchMoreResult.allPersons)
              };
          }
        |}
      ],
      (),
    )
    |> Js.Promise.then_(_ =>
         skipRef->React.Ref.setCurrent(skip) |> Js.Promise.resolve
       )
    |> ignore;
  };

  <div>
    {switch (full) {
     | {loading: true, data: None} => <p> {React.string("Loading...")} </p>
     | {data: Some(data)} =>
       <div>
         {data##allPersons
          ->Belt.Array.map(person =>
              <div key=person##id> {React.string(person##name)} </div>
            )
          |> ReasonReact.array}
         <button
           onClick=handleLoadMore
           disabled={full.networkStatus === ReasonApolloHooks.Types.FetchMore}>
           {React.string("Load more")}
         </button>
       </div>
     | {error: Some(_)} => <p> {React.string("Error")} </p>
     }}
  </div>;
};
