"""
Говрою сразу
это лишь идея
не я отвечаю за клиент
но я предложил че то 
так что вам решать че то
там 
далее
далее
стоп
ок

"""

from dataclasses import dataclass, asdict
from abc import ABC
import json

# ABstract Class
class BaseDTO(ABC):

    def to_dict(self):
        return asdict(self)

# потом в другой блят файле создаем классы с декоратором @dataclass
# чтобы to_dict сработала
# примеро
# @dataclass
# class CreateUserDTO(BaseDTO):
#   username: str
#   password: str
# и все, у черта cud = CreateUserDTO(), будет метод cud.to_dict(), потм его
# в джейсОн json.dumps(cud.to_dict())


# теперь в этой суке, надо формировать реквест прям по правилам по христиански амин
class Request:
    def __init__(self, method: str, route: str, payload: BaseDTO | None = None):
        self.raw = ""
        self.method = ""
        self.payload = payload.to_dict() if payload else dict()

    def raw_request(self) -> str:
        # ретерним реквест прям строкой
        ...

    def raw_payload(self) -> str:
        # возвр пейлод прям стркой
        ...


class Response:
    def __init__(self, raw:str):
        # получаем сырой респонс, должны превратить его данные в словарь(жиесть)
        self.data: dict = dict()
        # еще нужно знать какой статус у ответа, я потом подкорректирую с двумя р коды на серваке
        self.status = 2000


"""
Итак, текст ФИО еще раз заставил меня убедиться в том, что …
что это лишь наброски, но этого функционала достаточно, чтобы формировать 
реквесты и отправлять их уже через socket.write()
✌️

я тут еще посмотрел, есть какой QNetworkManager если не впадлу можно будет поробоват
но я думаю всем впадлу поэтому QTCPSocket в помощь, но есть
проблем  одна
там короче же есть readyread так вот, сообщение может прийти в нескольких ответах
и это просто не срабоает, поэтому надо реализовать класс TcpClient
которй будет чекать че там по content-length и смотреть чтобы прочитали ровно тсолько 
сколь там будет
чур не я

"""