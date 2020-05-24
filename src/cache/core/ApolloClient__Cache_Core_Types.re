module Graphql = ApolloClient__Graphql;
module DataProxy = {
  module Query = {
    module Js_ = {
      // interface Query<TVariables> {
      //   query: DocumentNode;
      //   variables?: TVariables;
      //   id?: string;
      // }
      type t('variables) = {
        query: Graphql.documentNode,
        variables: option('variables),
        id: option(string),
      };
    };

    type t('variables) = Js_.t('variables);
  };

  module Fragment = {
    module Js_ = {
      // interface Fragment<TVariables> {
      //   id: string;
      //   fragment: DocumentNode;
      //   fragmentName?: string;
      //   variables?: TVariables;
      // }
      type t('variables) = {
        id: string,
        fragment: Graphql.documentNode,
        fragmentName: option(string),
        variables: option('variables),
      };
    };

    type t('variables) = Js_.t('variables);
  };

  module WriteQueryOptions = {
    module Js_ = {
      // interface WriteQueryOptions<TData, TVariables> extends Query<TVariables> {
      //   data: TData;
      //   broadcast?: boolean;
      // }
      type t('jsData, 'variables) = {
        data: 'jsData,
        broadcast: option(bool),
        // ...extends Query
        query: Graphql.documentNode,
        variables: option('variables),
        id: option(string),
      };
    };

    type t('data, 'variables) = {
      data: 'data,
      broadcast: option(bool),
      query: Graphql.documentNode,
      variables: option('variables),
      id: option(string),
    };

    let toJs:
      (t('data, 'variables), ~parse: 'jsData => 'data) =>
      Js_.t('jsData, 'variables) =
      (t, ~parse) => {
        data: t.data->parse,
        broadcast: t.broadcast,
        query: t.query,
        variables: t.variables,
        id: t.id,
      };
  };

  module WriteFragmentOptions = {
    module Js_ = {
      // interface WriteFragmentOptions<TData, TVariables> extends Fragment<TVariables> {
      //    data: TData;
      //    broadcast?: boolean;
      // }
      type t('jsData, 'variables) = {
        data: 'jsData,
        broadcast: option(bool),
        // ...extends Fragment
        id: string,
        fragment: Graphql.documentNode,
        fragmentName: option(string),
        variables: option('variables),
      };
    };

    type t('data, 'variables) = {
      data: 'data,
      broadcast: option(bool),
      id: string,
      fragment: Graphql.documentNode,
      fragmentName: option(string),
      variables: option('variables),
    };
  };
};
