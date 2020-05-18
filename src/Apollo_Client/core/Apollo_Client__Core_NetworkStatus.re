type t =
  | Loading
  | SetVariables
  | FetchMore
  | Refetch
  | Poll
  | Ready
  | Error
  | Unknown;

let fromJs =
  fun
  | 1 => Loading
  | 2 => SetVariables
  | 3 => FetchMore
  | 4 => Refetch
  | 6 => Poll
  | 7 => Ready
  | 8 => Error
  | _ => Unknown;
