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

  <div className="person-list">
    {switch (full) {
     | {loading: true, data: None} => <p> {React.string("Loading...")} </p>
     | {data: Some(data)} =>
       <>
         {data##allPersons
          ->Belt.Array.map(person =>
              <div key=person##id className="person">
                <div className="person-field">
                  <span className="person-label">
                    {React.string("Id: ")}
                  </span>
                  {React.string(person##id)}
                </div>
                <div className="person-field">
                  <span className="person-label">
                    {React.string("Name: ")}
                  </span>
                  {React.string(person##name)}
                </div>
                <div className="person-field">
                  <span className="person-label">
                    {React.string("Age: ")}
                  </span>
                  {person##age |> string_of_int |> React.string}
                </div>
              </div>
            )
          |> React.array}
         <button
           onClick=handleLoadMore
           disabled={full.networkStatus === ReasonApolloHooks.Types.FetchMore}>
           {React.string("Load more")}
         </button>
       </>
     | {error: Some(_)} => <p> {React.string("Error")} </p>
     | {error: None, data: None, loading: false} =>
       <p> {React.string("Not asked")} </p>
     }}
  </div>;
};
