function formatPortugueseDate(date) {
  if (!date) return '';
  
  const dateObj = new Date(date);
  
  // Usar timezone de Lisboa/Portugal explicitamente
  return dateObj.toLocaleDateString('pt-PT', {
    timeZone: 'Europe/Lisbon',
    year: 'numeric',
    month: '2-digit',
    day: '2-digit'
  });
}


function formatPortugueseTime(date) {
  if (!date) return '';
  
  const dateObj = new Date(date);
  
  // Usar timezone de Lisboa/Portugal explicitamente
  return dateObj.toLocaleTimeString('pt-PT', {
    timeZone: 'Europe/Lisbon',
    hour: '2-digit',
    minute: '2-digit'
  });
}


function formatPortugueseDateTime(date) {
  if (!date) return '';
  
  const dateObj = new Date(date);
  
  // Usar timezone de Lisboa/Portugal explicitamente
  const formattedDate = dateObj.toLocaleDateString('pt-PT', {
    timeZone: 'Europe/Lisbon',
    year: 'numeric',
    month: '2-digit',
    day: '2-digit'
  });
  
  const formattedTime = dateObj.toLocaleTimeString('pt-PT', {
    timeZone: 'Europe/Lisbon',
    hour: '2-digit',
    minute: '2-digit'
  });
  
  return `${formattedDate} às ${formattedTime}`;
}


function formatPortugueseDateLong(date) {
  if (!date) return '';
  
  const dateObj = new Date(date);
  
  // Usar timezone de Lisboa/Portugal explicitamente
  return dateObj.toLocaleDateString('pt-PT', {
    timeZone: 'Europe/Lisbon',
    year: 'numeric',
    month: 'long',
    day: 'numeric'
  });
}

// Tornar as funções disponíveis globalmente
window.formatPortugueseDate = formatPortugueseDate;
window.formatPortugueseTime = formatPortugueseTime;
window.formatPortugueseDateTime = formatPortugueseDateTime;
window.formatPortugueseDateLong = formatPortugueseDateLong;
