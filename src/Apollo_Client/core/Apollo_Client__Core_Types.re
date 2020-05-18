module Graphql = Apollo_Client__Graphql;

module ApolloQueryResult = {
  type t('parsedData) = {
    data: option('parsedData),
    errors: option(array(Graphql.Error.GraphQLError.t)),
    loading: bool,
    networkStatus: int,
  };

  let fromJs: (t('jsData), ~parse: 'jsData => 'parsedData) => t('parsedData) =
    ({data, errors, loading, networkStatus}, ~parse) => {
      data: data->Belt.Option.map(parse),
      errors,
      loading,
      networkStatus,
    };
};
