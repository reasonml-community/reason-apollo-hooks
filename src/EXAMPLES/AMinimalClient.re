let graphqlEndpoint = "";

let headers = {"high": "five"};

let httpLink =
  ApolloClient.HttpLink.make(
    ~uri=_ => "http://" ++ graphqlEndpoint,
    ~credentials="include",
    ~headers=Obj.magic(headers),
    (),
  );

let wsLink =
  ApolloLinkWs.(
    WebSocketLink.make(
      ~uri="ws://" ++ graphqlEndpoint,
      ~options=
        ClientOptions.make(
          ~connectionParams=
            ConnectionParams(Obj.magic({"headers": headers})),
          ~reconnect=true,
          (),
        ),
      (),
    )
  );

let terminatingLink =
  ApolloClient.split(
    ~test=
      ({query}) => {
        let definition = ApolloClient.Utilities.getOperationDefinition(query);
        switch (definition) {
        | Some({kind, operation}) =>
          kind === "OperationDefinition" && operation === "subscription"
        | None => false
        };
      },
    ~right=httpLink,
    ~left=wsLink,
  );

let apolloClient =
  ApolloClient.ApolloClient.(
    make(
      ~cache=ApolloClient.InMemoryCache.make(),
      ~connectToDevTools=true,
      ~defaultOptions=
        DefaultOptions.make(
          ~mutate=
            DefaultOptions.DefaultMutationOptions.make(
              ~awaitRefetchQueries=true,
              ~fetchPolicy=NetworkOnly,
              ~errorPolicy=All,
              (),
            ),
          ~query=
            DefaultOptions.DefaultQueryOptions.make(
              ~fetchPolicy=NetworkOnly,
              ~errorPolicy=All,
              (),
            ),
          ~watchQuery=
            DefaultOptions.DefaultWatchQueryOptions.make(
              ~fetchPolicy=NetworkOnly,
              ~errorPolicy=All,
              (),
            ),
          (),
        ),
      ~link=terminatingLink,
      (),
    )
  );
