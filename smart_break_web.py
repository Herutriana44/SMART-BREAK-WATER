import streamlit as st
import pandas as pd
import pyrebase
from datetime import datetime
import plotly.graph_objects as go
from io import BytesIO

def convert_to_date(date_string):
  # Format tanggal input
  input_format = "%a %b %d %H:%M:%S %Y"

  # Konversi string ke objek datetime
  datetime_object = datetime.strptime(date_string, input_format)

  return datetime_object

# Konfigurasi Firebase
config = {
    "apiKey": "AIzaSyBVvZ3B-l596NQ4APH3u5DPGsy2z0EJ45I",
    "authDomain": "adefirebase-b54f6.firebaseapp.com",
    "databaseURL": "https://adefirebase-b54f6-default-rtdb.firebaseio.com",
    "storageBucket": "adefirebase-b54f6.appspot.com"
}

# Inisialisasi Firebase
firebase = pyrebase.initialize_app(config)

# Dapatkan referensi ke database
db = firebase.database()

base_data = db.child("Smart-Break-Water").get().each()
water_0 = base_data[0].val()
water_1 = base_data[1].val()
ground_0 = base_data[2].val()
tingkat_bahaya = base_data[3].val()
status_bahaya = base_data[4].val()
date_time = base_data[5].val()

def create_bar_chart_streamlit(data, y_column):
    # Menetapkan warna berdasarkan status_bahaya
    color_map = {
        'Rendah': 'green',
        'Sedang': 'yellow',
        'Tinggi': 'red'
    }
    
    # Membuat list warna untuk setiap bar
    colors = [color_map[status] for status in data['status_bahaya']]
    
    # Membuat grafik menggunakan Plotly
    fig = go.Figure(data=[
        go.Bar(
            x=data['date_time'],
            y=data[y_column],
            marker_color=colors
        )
    ])
    
    # Menyesuaikan tampilan grafik
    fig.update_layout(
        title=f'Grafik {y_column} berdasarkan Tanggal',
        xaxis_title='Tanggal',
        yaxis_title=y_column,
        xaxis_tickangle=-45
    )
    
    # Menambahkan legenda
    for status, color in color_map.items():
        fig.add_trace(go.Bar(
            x=[None],
            y=[None],
            name=status,
            marker_color=color
        ))
    
    # Menampilkan grafik di Streamlit
    st.plotly_chart(fig)

def to_excel(df):
    output = BytesIO()
    with pd.ExcelWriter(output, engine='xlsxwriter') as writer:
        df.to_excel(writer, index=False, sheet_name='Sheet1')
    processed_data = output.getvalue()
    return processed_data

# Judul aplikasi
st.title("Dashboard Monitoring Sensor")

# Menghasilkan data simulasi
data = pd.DataFrame({
    'water_0': water_0,
    'water_1': water_1,
    'ground_0': ground_0,
    'tingkat_bahaya': tingkat_bahaya,
    'status_bahaya': status_bahaya,
    'date_time': date_time
})

# convert date_time column to date format
data['date_time'] = [convert_to_date(date) for date in data['date_time']]

# Grafik 1: Suhu
st.subheader("Grafik Ground")
create_bar_chart_streamlit(data, "ground_0")

# Grafik 2: Kelembaban
st.subheader("Grafik Water 1")
create_bar_chart_streamlit(data, "water_0")

# Grafik 3: Tekanan Udara
st.subheader("Grafik Water 2")
create_bar_chart_streamlit(data, "water_1")

if st.button('Unduh Data sebagai Excel'):
    excel_data = to_excel(data)
    st.download_button(
        label="Klik untuk mengunduh",
        data=excel_data,
        file_name="data.xlsx",
        mime="application/vnd.openxmlformats-officedocument.spreadsheetml.sheet"
    )

# Menampilkan data mentah (opsional)
if st.checkbox("Tampilkan data mentah"):
    st.write(data)