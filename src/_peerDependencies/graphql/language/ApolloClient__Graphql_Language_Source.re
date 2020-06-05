type location = {
  line: int,
  column: int,
};

type t = {
  body: string,
  name: string,
  locationOffset: location,
  //   constructor(body: string, name?: string, locationOffset?: Location);
};
