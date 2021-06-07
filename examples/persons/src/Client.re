let inMemoryCache = ApolloInMemoryCache.createInMemoryCache();

let httpLink =
  ApolloLinks.createHttpLink(
    ~uri="https://reason-apollo-hooks-example.herokuapp.com/v1/graphql",
    (),
  );

/* WebSocket client */
let webSocketLink =
  ApolloLinks.webSocketLink({
    uri: "wss://reason-apollo-hooks-example.herokuapp.com/v1/graphql",
    options: {
      reconnect: true,
      connectionParams: None,
    },
  });

/* Using the ability to split links, you can send data to each link
   depending on what kind of operation is being sent */
let webSocketHttpLink =
  ApolloLinks.split(
    operation => {
      let operationDefition =
        ApolloUtilities.getMainDefinition(operation.query);
      operationDefition.kind == "OperationDefinition"
      && operationDefition.operation == "subscription";
    },
    webSocketLink,
    httpLink,
  );

let instance =
  ReasonApollo.createApolloClient(
    ~link=webSocketHttpLink,
    ~cache=inMemoryCache,
    (),
  );
