// Import necessary modules from Firebase SDKs
const functions = require('firebase-functions');
const admin = require('firebase-admin');
const serviceAccount = require('./config/serviceAccountPrivateKey.json'); 


// Initialize Firebase Admin SDK
admin.initializeApp({
  credential: admin.credential.cert(serviceAccount),
  databaseURL: "https://cs145-0x0-b0x0-labs-berdebox-default-rtdb.asia-southeast1.firebasedatabase.app"
});

// Cloud Function to send a notification when doorbell button is pressed
exports.sendDoorbellNotification = functions.database
  .ref('/berdebox1/input/doorbell_button_is_pressed')
  .onUpdate(async (change) => {
    // Data contains a snapshot of the data updated or deleted
    const beforeUpdate = change.before.val();
    const afterUpdate = change.after.val();

    // Check if the doorbell button has changed from false to true
    if (beforeUpdate === false && afterUpdate === true) {

      // Assuming you have a topic or a list of tokens to send notifications to
      const topic = 'doorbell-alerts';

      // Construct the notification payload
      const payload = {
        notification: {
          title: 'Looks like your delivery is here!',
          body: 'Someone rang your doorbell. Open the app to permit.',
        },
        topic: topic
      };


      try {
        // Send the notification
        console.log('Sending a notification!');
        const response = await admin.messaging().send(payload);
        console.log('Successfully sent message:', response);
      } catch (error) {
        console.error('Error sending message:', error);
      }
    }
  });
