FROM tensorflow/tensorflow
RUN pip install numpy pandas scikit-learn
COPY ./ /home/server
CMD cd /home/server && python3 server.py 

