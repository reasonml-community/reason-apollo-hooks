module Graphql = Apollo_Client__Graphql;

module ApolloQueryResult = {
  module Raw = {
    type t('raw_tData) = {
      data: Js.nullable('raw_tData),
      errors: Js.nullable(array(Graphql.Error.GraphQLError.t)),
      loading: bool,
      networkStatus: int,
    };
  };

  type t('tData) = {
    data: option('tData),
    errors: option(array(Graphql.Error.GraphQLError.t)),
    loading: bool,
    networkStatus: int,
  };

  let fromRaw =
      ({data, errors, loading, networkStatus}: Raw.t('raw_tData), ~parse) => {
    data: data->Js.toOption->Belt.Option.map(parse),
    errors: errors->Js.toOption,
    loading,
    networkStatus,
  };
};
