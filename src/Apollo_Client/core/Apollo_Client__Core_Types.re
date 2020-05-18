module Graphql = Apollo_Client__Graphql;

module ApolloQueryResult = {
  module JS = {
    type t('jsData) = {
      data: Js.nullable('jsData),
      errors: Js.nullable(array(Graphql.Error.GraphQLError.t)),
      loading: bool,
      networkStatus: int,
    };
  };

  type t('parsedData) = {
    data: option('parsedData),
    errors: option(array(Graphql.Error.GraphQLError.t)),
    loading: bool,
    networkStatus: int,
  };

  let fromJs =
      ({data, errors, loading, networkStatus}: JS.t('jsData), ~parse) => {
    data: data->Js.toOption->Belt.Option.map(parse),
    errors: errors->Js.toOption,
    loading,
    networkStatus,
  };
};
