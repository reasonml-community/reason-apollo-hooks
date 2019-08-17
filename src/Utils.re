let toQueryObj = (result): ApolloClient.queryObj => {
  "query": ApolloClient.gql(. result##query),
  "variables": result##variables,
};

let toReadQueryOptions = result => {
  "query": ApolloClient.gql(. result##query),
  "variables": Js.Nullable.fromOption(Some(result##variables)),
};
