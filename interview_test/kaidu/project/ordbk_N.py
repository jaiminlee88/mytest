from base import BaseBroker
from typing import Any
import csv, sys
from sortedcontainers import SortedDict
from sortedcontainers import SortedList
from datetime import datetime
MAX_INT = sys.maxsize
class param:
    idx = 0
    time = 1
    price = 2
    volume = 3
    quote_type = 4
    order_type = 5

class BookItem:
    def __init__(self, _time, _idx, _volume):
        self.time = _time
        self.idx = _idx
        self.volume = _volume

    def __repr__(self):
        return f'BookItem({self.time}, {self.idx}, {self.volume})'

    def __lt__(self, other):
        return self.time < other.time or self.idx < other.idx

class ordbk_N(BaseBroker):
    def __init__(self) -> None:
        self.ask_que = SortedDict()
        self.bid_que = SortedDict()

    def order_book(self, level: int = 5, **kwargs):
        # just print the order book ???
        ask_que_info = f'[ask]: '
        ask_level = 0
        for px in self.ask_que:
            if ask_level >= level:
                break
            ask_que_info += f'  {px}->|'
            for item in self.ask_que[px]:
                ask_que_info += f'{item.__repr__()},'
            ask_level += 1

        bid_que_info = f'[bid]: '
        bid_level = 0
        for px in reversed(self.bid_que):
            if bid_level >= level:
                break
            bid_que_info += f'  {px}->|'
            for item in self.bid_que[px]:
                bid_que_info += f'{item.__repr__()},'
            bid_level += 1
        print(ask_que_info)
        print(bid_que_info)

    def fill_order(self, order) -> list:
        if order[param.quote_type] == 'ASK':
            que = self.bid_que
            px_seq = list(reversed(que.keys()))
            if len(que) == 0 or order[param.price] > que.peekitem(-1)[0]:
                return order
        else:
            que = self.ask_que
            px_seq = list(que.keys())
            if len(que) == 0 or order[param.price] < que.peekitem(0)[0]:
                return order
        for px in px_seq:
            if order[param.quote_type] == 'ASK' and px < order[param.price]:
                break
            if order[param.quote_type] == 'BID' and px > order[param.price]:
                break
            while True:
                if len(que[px]) == 0:
                    que.pop(px)
                    break
                if order[param.volume] == 0:
                    break
                if que[px][0].volume <= order[param.volume]:
                    order[param.volume] -= que[px][0].volume
                    que[px].pop(0)
                else:
                    que[px][0].volume -= order[param.volume]
                    order[param.volume] = 0
            if order[param.volume] == 0:
                break
        return order

    def push_order(self, order:list, que:SortedDict):
        if order[param.volume] <= 0:
            return
        if order[param.price] not in que:
            que[order[param.price]] = SortedList([
                BookItem(order[param.time], order[param.idx], order[param.volume])
            ])
            return
        que[order[param.price]].add(
           BookItem(order[param.time], order[param.idx], order[param.volume])
        )

    def transact(self, order: Any, **kwargs):
        if (len(order) != 6
            or order[param.quote_type] not in ['ASK', 'BID']
            or order[param.order_type] not in ['MARKET', 'LIMIT']
            ):
            print('Invalid order:', order)
            return
        order[param.volume] = int(order[param.volume])
        order[param.price] = float(order[param.price])
        if  (order[param.order_type] == 'LIMIT' and order[param.price] <= 0) \
            or order[param.volume] <= 0 \
            or (order[param.order_type] == 'BID' and order[param.volume] % 100 != 0):
            print('Invalid order:', order)
            return
        if order[param.order_type] == 'MARKET':
            order[param.price] = MAX_INT if order[param.quote_type] == 'BID' else 0
        order = self.fill_order(order)
        if order[param.order_type] == 'MARKET':
            return
        if order[param.quote_type] == 'ASK':
            self.push_order(order, self.ask_que)
        elif order[param.quote_type] == 'BID':
            self.push_order(order, self.bid_que)

if __name__ == '__main__':
    ordbook = ordbk_N()
    with open('_order.csv', mode='r') as file:
        reader = csv.reader(file)
        next(reader)
        st = datetime.now()
        for row in reader:
            # print(f'[ord]:   {row}')
            # odd lot is ignored in bid side, details were showed in codes
            ordbook.transact(order = row)
            # ordbook.order_book()
        print('done====================')
        ordbook.order_book()
        print(datetime.now() - st)