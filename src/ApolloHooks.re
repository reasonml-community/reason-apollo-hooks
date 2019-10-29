module Mutation = ApolloHooksMutation;
module Query = ApolloHooksQuery;
module ApolloProvider = ApolloHooksApolloProvider;

let useQuery = Query.useQuery;
let useMutation = Mutation.useMutation;
let useDynamicMutation = Mutation.useDynamicMutation;

let toQueryObj = (result): ApolloClient.queryObj => {
  "query": ApolloClient.gql(. result##query),
  "variables": result##variables,
};

let toReadQueryOptions = result => {
  "query": ApolloClient.gql(. result##query),
  "variables": Js.Nullable.fromOption(Some(result##variables)),
};