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

  let (_simple, full) =
    GetAllPersonsQuery.use(
      ~variables=getAllPersons##variables,
      ~notifyOnNetworkStatusChange=true,
      (),
    );

  let handleLoadMore = _ => {
    let skip = React.Ref.current(skipRef) + personsPerPage;
    skipRef->React.Ref.setCurrent(skip);

    let getNextPage =
      GetAllPersonsConfig.make(~skip, ~first=personsPerPage, ());

    full.fetchMore(
      ~variables=getNextPage##variables,
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
     | {error: None, data: None, loading: false} =>
       <p> {React.string("Not asked")} </p>
     }}
  </div>;
};
