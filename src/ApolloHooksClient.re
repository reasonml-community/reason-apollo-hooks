open ApolloHooksTypes;

type queryError = {. "message": string};
type queryResult('a) = {
  data: option('a),
  errors: option(array(queryError)),
  loading: bool,
  networkStatus: ApolloHooksTypes.networkStatus,
  stale: bool,
};

[@bs.module "graphql-tag"] external gql: ReasonApolloTypes.gql = "default";

type options('data);
[@bs.obj]
external doMakeOptions:
  (
    ~query: ReasonApolloTypes.queryString,
    ~variables: 'raw_t_variables=?,
    ~notifyOnNetworkStatusChange: bool=?,
    ~fetchPolicy: string=?,
    ~errorPolicy: string=?,
    ~pollInterval: int=?,
    unit
  ) =>
  options('data);

let makeOptions =
    (
      ~variables: 'raw_t_variables=?,
      ~notifyOnNetworkStatusChange=?,
      ~fetchPolicy=?,
      ~errorPolicy=?,
      ~pollInterval=?,
      (_, query, _): graphqlDefinition('data, _),
    ) => {
  doMakeOptions(
    ~query=gql(. query),
    ~variables?,
    ~notifyOnNetworkStatusChange?,
    ~fetchPolicy=?fetchPolicy->Belt.Option.map(f => fetchPolicyToJs(f)),
    ~errorPolicy=?errorPolicy->Belt.Option.map(e => errorPolicyToJs(e)),
    ~pollInterval?,
    (),
  );
};

[@bs.send]
external query:
  (ApolloClient.t, options('data)) => Js.Promise.t(queryResult('data)) =
  "query";

[@bs.module "../../../apolloClient"]
external client: ApolloClient.t = "default";
