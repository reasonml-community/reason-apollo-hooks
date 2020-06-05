module ZenObservable = {
  // interface SubscriptionObserver<T> {
  //     closed: boolean;
  //     next(value: T): void;
  //     error(errorValue: any): void;
  //     complete(): void;
  // }

  // type Subscriber<T> = (observer: SubscriptionObserver<T>) => void | (() => void) | Subscription;

  // interface ObservableLike<T> {
  //     subscribe?: Subscriber<T>;
  //     [Symbol.observable](): Observable<T> | ObservableLike<T>;
  // }

  // interface Subscription {
  //     closed: boolean;
  //     unsubscribe(): void;
  // }
  type subscription = {
    closed: bool,
    unsubscribe: unit => unit,
  };

  // interface Observer<T> {
  //     start?(subscription: Subscription): any;
  //     next?(value: T): void;
  //     error?(errorValue: any): void;
  //     complete?(): void;
  // }
  type observer('t) = {
    start: option(subscription => unit),
    next: option('t => unit),
    error: option(Js.Json.t => unit),
    complete: option(unit => unit),
  };
};

module Observable = {
  // declare class Observable<T> {
  //     constructor(subscriber: ZenObservable.Subscriber<T>)

  //     subscribe(observer: ZenObservable.Observer<T>): ZenObservable.Subscription;
  //     subscribe(onNext: (value: T) => void, onError?: (error: any) => void, onComplete?: () => void): ZenObservable.Subscription;

  //     [Symbol.observable](): Observable<T>;

  //     forEach(callback: (value: T) => void): Promise<void>;
  //     map<R>(callback: (value: T) => R): Observable<R>;
  //     filter(callback: (value: T) => boolean): Observable<T>;
  //     reduce(callback: (previousValue: T, currentValue: T) => T, initialValue?: T): Observable<T>;
  //     reduce<R>(callback: (previousValue: R, currentValue: T) => R, initialValue?: R): Observable<R>;
  //     flatMap<R>(callback: (value: T) => ZenObservable.ObservableLike<R>): Observable<R>;
  //     concat<R>(...observable: Array<Observable<R>>): Observable<R>;

  //     static from<R>(observable: Observable<R> | ZenObservable.ObservableLike<R> | ArrayLike<R>): Observable<R>;
  //     static of<R>(...items: R[]): Observable<R>;
  // }
  type t('t);

  [@bs.send]
  external subscribe:
    (
      t('t),
      ~onNext: 't => unit,
      ~onError: Js.Json.t => unit=?,
      ~onComplete: unit => unit=?,
      unit
    ) =>
    ZenObservable.subscription =
    "subscribe";

  [@bs.send]
  external subscribeWithObserver:
    (t('t), ZenObservable.observer('t)) => ZenObservable.subscription =
    "subscribe";
};
