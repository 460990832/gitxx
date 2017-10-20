from data_util import DataUtils
from mxnet import gluon
from mxnet import ndarray as nd
from mxnet import autograd
import mxnet as mx

def getdata():
    trainfile_X = 'train-images.idx3-ubyte'
    trainfile_y = 'train-labels.idx1-ubyte'
    testfile_X = 't10k-images.idx3-ubyte'
    testfile_y = 't10k-labels.idx1-ubyte'
    train_X = DataUtils(filename=trainfile_X).getImage()
    train_y = DataUtils(filename=trainfile_y).getLabel()
    test_X = DataUtils(testfile_X).getImage()
    test_y = DataUtils(testfile_y).getLabel()
    return train_X, train_y, test_X, test_y

def accuracy(output, label):
    return nd.mean(output.argmax(axis=1)==label).asscalar()

def evaluate_accuracy(data_iterator, net, ctx=mx.cpu()):
    acc = 0.
    for data, label in data_iterator:
        output = net(data.as_in_context(ctx))
        acc += accuracy(output, label.as_in_context(ctx))
    return acc / len(data_iterator)


net = gluon.nn.Sequential()
with net.name_scope():
    net.add(gluon.nn.Flatten())
    net.add(gluon.nn.Dense(256, activation="relu"))
    net.add(gluon.nn.Dense(10))
net.initialize()


batch_size=256
train_X, train_y, test_X, test_y = getdata()
train_set = gluon.data.ArrayDataset(nd.array(train_X), nd.array(train_y))
test_set = gluon.data.ArrayDataset(nd.array(test_X), nd.array(test_y))
train_data = gluon.data.DataLoader(train_set, batch_size, shuffle=True)
test_data = gluon.data.DataLoader(test_set, batch_size, shuffle=False)


softmax_cross_entropy = gluon.loss.SoftmaxCrossEntropyLoss()
trainer = gluon.Trainer(net.collect_params(), 'sgd', {'learning_rate': 0.5})

for epoch in range(5):
    train_loss = 0.
    train_acc = 0.
    for data,label in train_data:
        with autograd.record():
            output = net(data)
            loss = softmax_cross_entropy(output, label)
        loss.backward()
        trainer.step(batch_size)

        train_loss += nd.mean(loss).asscalar()
        train_acc += accuracy(output, label)

    test_acc = evaluate_accuracy(test_data, net)
    print("Epoch %d. Loss: %f, Train acc %f, Test acc %f" % (
        epoch, train_loss / len(train_data), train_acc / len(train_data), test_acc))

import cv2
img1=cv2.imread('1.jpg')
b,g,r = cv2.split(img1)
test=b<245
test=test+0
print(test)
print(net(nd.array(test).reshape([1,784])))

