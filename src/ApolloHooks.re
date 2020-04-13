module Mutation = ApolloHooksMutation;
module Query = ApolloHooksQuery;
module Provider = ApolloHooksProvider;
module Subscription = ApolloHooksSubscription;

let useQuery = Query.useQuery;
let useMutation = Mutation.useMutation;
let useSubscription = Subscription.useSubscription;

let toQueryObj = (result): ApolloClient.queryObj => {
  query: ApolloClient.gql(. result##query),
  variables: result##variables,
};

let toQueryObj2 = (query, variables): ApolloClient.queryObj => {
  query: ApolloClient.gql(. query),
  variables,
};

let toReadQueryOptions = result => {
  "query": ApolloClient.gql(. result##query),
  "variables": Js.Nullable.fromOption(Some(result##variables)),
};

external toOptimisticResult: 'a => Mutation.optimisticResult = "%identity";
