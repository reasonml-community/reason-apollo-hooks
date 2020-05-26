// export interface ApolloProviderProps<TCache> {
//     client: ApolloClient<TCache>;
//     children: React.ReactNode | React.ReactNode[] | null;
// }
// export declare const ApolloProvider: React.FC<ApolloProviderProps<any>>;
[@bs.module "@apollo/client"]
external make:
  (~client: ApolloClient__ApolloClient.t, ~children: React.element) =>
  React.element =
  "ApolloProvider";
