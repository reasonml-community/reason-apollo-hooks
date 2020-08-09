module Types = ApolloClient_Types;

type queryError = {. "message": string};
type queryResult('a) = {
  data: option('a),
  errors: option(array(queryError)),
  loading: bool,
  networkStatus: Types.networkStatus,
  stale: bool,
};

[@bs.module "graphql-tag"] external gql: Types.gql = "default";

type options('data);
[@bs.obj]
external doMakeOptions:
  (
    ~query: Types.queryString,
    ~variables: 'raw_t_variables=?,
    ~notifyOnNetworkStatusChange: bool=?,
    ~fetchPolicy: string=?,
    ~errorPolicy: string=?,
    ~pollInterval: int=?,
    unit
  ) =>
  options('data);

let makeOptions:
  type t raw_t t_variables raw_t_variables.
    (
      ~variables: t_variables=?,
      ~notifyOnNetworkStatusChange: bool=?,
      ~fetchPolicy: Types.fetchPolicy=?,
      ~errorPolicy: Types.errorPolicy=?,
      ~pollInterval: int=?,
      ~context: Types.Context.t=?,
      (module Types.Operation with
         type t = t and
         type Raw.t = raw_t and
         type t_variables = t_variables and
         type Raw.t_variables = raw_t_variables)
    ) =>
    options(t) =
  (
    ~variables=?,
    ~notifyOnNetworkStatusChange=?,
    ~fetchPolicy=?,
    ~errorPolicy=?,
    ~pollInterval=?,
    ~context=?,
    (module Operation),
  ) => {
    doMakeOptions(
      ~query=gql(. Operation.query),
      ~variables=?{switch(variables) {
        | Some(variables) => Some(Operation.serializeVariables(variables))
        | None => None}},
      ~notifyOnNetworkStatusChange?,
      ~fetchPolicy=?
        fetchPolicy->Belt.Option.map(f => Types.fetchPolicyToJs(f)),
      ~errorPolicy=?
        errorPolicy->Belt.Option.map(e => Types.errorPolicyToJs(e)),
      ~pollInterval?,
      (),
    );
  };

[@bs.send]
external query:
  (ApolloClient_Client.t, options('data)) =>
  Js.Promise.t(queryResult('data)) =
  "query";
