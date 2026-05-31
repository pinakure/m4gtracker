FROM python:3.12-slim

# Evita que Python escriba archivos .pyc y almacene en búfer de salida
ENV PYTHONDONTWRITEBYTECODE=1
ENV PYTHONUNBUFFERED=1

# Crear el usuario y grupo 'melodify'
RUN groupadd -r m4gtracker && useradd -r -g m4gtracker m4gtracker

WORKDIR /src/m4gtracker

# Instalar dependencias como root
COPY requirements.txt /src/m4gtracker

RUN pip install --no-cache-dir --upgrade pip && \
    pip install --no-cache-dir -r /src/m4gtracker/requirements.txt

# Crear estructura necesaria
RUN mkdir -p \
    /static/m4gtracker \
    /media/m4gtracker 

# Copiar proyecto directamente con ownership correcto
COPY --chown=m4gtracker:m4gtracker . /src/m4gtracker

# Cambiar a usuario no-root
#USER m4gtracker

# Ejecutar migraciones, recopilar estáticos y arrancar Gunicorn
CMD ["sh", "-c", "python manage.py makemigrations && python manage.py migrate && python manage.py collectstatic --noinput && gunicorn m4gtracker.wsgi:application --bind 0.0.0.0:8000"]
