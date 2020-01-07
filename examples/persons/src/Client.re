let inMemoryCache = ApolloInMemoryCache.createInMemoryCache();

let httpLink =
  ApolloLinks.createHttpLink(
    ~uri="https://api.graph.cool/simple/v1/cjdgba1jw4ggk0185ig4bhpsn",
    (),
  );

/* WebSocket client */
let webSocketLink =
  ApolloLinks.webSocketLink(
    ~uri="wss://subscriptions.graph.cool/v1/cjdgba1jw4ggk0185ig4bhpsn",
    ~reconnect=true,
    (),
  );

/* Using the ability to split links, you can send data to each link
   depending on what kind of operation is being sent */
let webSocketHttpLink =
  ApolloLinks.split(
    operation => {
      let operationDefition =
        ApolloUtilities.getMainDefinition(operation##query);
      operationDefition##kind == "OperationDefinition"
      &&
      operationDefition##operation == "subscription";
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
