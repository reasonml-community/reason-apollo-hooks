[@bs.deriving jsConverter]
type t =
  | [@bs.as 1] Loading
  | [@bs.as 2] SetVariables
  | [@bs.as 3] FetchMore
  | [@bs.as 4] Refetch
  | [@bs.as 5] Poll
  | [@bs.as 6] Ready
  | [@bs.as 7] Error;

let toJs = tToJs;

let fromJs = string => tFromJs(string)->Belt.Option.getExn;
