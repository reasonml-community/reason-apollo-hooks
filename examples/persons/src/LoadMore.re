module GetAllPersonsQuery = [%graphql
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

let personsPerPage = 5;

[@react.component]
let make = () => {
  let (_simple, full) =
    ApolloHooks.useQuery(
      ~variables=
        GetAllPersonsQuery.makeVariables(~skip=0, ~first=personsPerPage, ()),
      ~notifyOnNetworkStatusChange=true,
      GetAllPersonsQuery.definition,
    );

  let handleLoadMore = _ => {
    let skip =
      switch (full) {
      | {data: Some(data)} => data##allPersons->Belt.Array.length
      | _ => 0
      };

    full.fetchMore(
      ~variables=
        GetAllPersonsQuery.makeVariables(~skip, ~first=personsPerPage, ()),
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
         <Persons persons={data##allPersons} />
         <button
           onClick=handleLoadMore disabled={full.networkStatus === FetchMore}>
           {React.string("Load more")}
         </button>
       </>
     | {error: Some(_)} => <p> {React.string("Error")} </p>
     | {error: None, data: None, loading: false} =>
       <p> {React.string("Not asked")} </p>
     }}
  </div>;
};
