import React from "react";
import * as Sentry from "@sentry/react-native";

import {
  SafeAreaView,
  ScrollView,
  StatusBar,
  StyleSheet,
  Text,
  useColorScheme,
  View,
} from "react-native";

import {
  Colors,
  DebugInstructions,
  Header,
  LearnMoreLinks,
  ReloadInstructions,
} from "react-native/Libraries/NewAppScreen";

const Section = ({ children, title }) => {
  const isDarkMode = useColorScheme && useColorScheme() === "dark";
  return (
    <View style={styles.sectionContainer}>
      <Text
        style={[
          styles.sectionTitle,
          {
            color: isDarkMode ? Colors.white : Colors.black,
          },
        ]}
      >
        {title}
      </Text>
      <Text
        style={[
          styles.sectionDescription,
          {
            color: isDarkMode ? Colors.light : Colors.dark,
          },
        ]}
      >
        {children}
      </Text>
    </View>
  );
};

Sentry.init({
  dsn:
    // Replace the example DSN below with your own DSN:
    'https://6890c2f6677340daa4804f8194804ea2@o19635.ingest.sentry.io/148053',
  debug: true,
  beforeSend: (e) => {
    if (!e.tags) {
      e.tags = {};
    }
    e.tags['beforeSend'] = 'JS layer';

    console.log('Event beforeSend:', e);

    // setEventId(e.event_id);

    return e;
  },
  enableAutoSessionTracking: true,
  // For testing, session close when 5 seconds (instead of the default 30) in the background.
  sessionTrackingIntervalMillis: 5000,
});

const App = () => {
  const isDarkMode = useColorScheme && useColorScheme() === "dark";

  const backgroundStyle = {
    backgroundColor: isDarkMode ? Colors.darker : Colors.lighter,
  };

  const hermes = global.HermesInternal ? (
    <View style={styles.engine}>
      <Text
        style={[
          styles.footer,
          {
            color: isDarkMode ? Colors.light : Colors.dark,
          },
        ]}
      >
        Engine: Hermes
      </Text>
    </View>
  ) : null;

  return (
    <SafeAreaView>
    <ScrollView
      contentInsetAdjustmentBehavior="automatic"
      style={styles.scrollView}>
      <Header />
      {global.HermesInternal == null ? null : (
        <View style={styles.engine}>
          <Text style={styles.footer}>Engine: Hermes</Text>
        </View>
      )}
      <View style={styles.body}>
        <View style={styles.sectionContainer}>
          <Text
            style={styles.sectionTitle}
            onPress={() => {
              Sentry.captureMessage('React Native Test Message');
            }}>
            captureMessage
          </Text>
          <Text
            style={styles.sectionTitle}
            onPress={() => {
              Sentry.captureException(new Error('captureException test'));
            }}>
            captureException
          </Text>
          <Text
            style={styles.sectionTitle}
            onPress={() => {
              throw new Error('throw new error test');
            }}>
            throw new Error
          </Text>
          <Text
            style={styles.sectionTitle}
            onPress={() => {
              Sentry.nativeCrash();
            }}>
            nativeCrash
          </Text>
          <Text style={styles.sectionTitle}>Step One</Text>
          <Text style={styles.sectionDescription}>
            Edit <Text style={styles.highlight}>App.js</Text> to change this
            screen and then come back to see your edits.
          </Text>
        </View>
        <View style={styles.sectionContainer}>
          <Text style={styles.sectionTitle}>Learn More</Text>
          <Text style={styles.sectionDescription}>
            Read the docs to discover what to do next:
          </Text>
        </View>
        <LearnMoreLinks />
      </View>
    </ScrollView>
  </SafeAreaView>
  );
};

const styles = StyleSheet.create({
  engine: {
    position: "absolute",
    right: 0,
  },
  sectionContainer: {
    marginTop: 32,
    paddingHorizontal: 24,
  },
  sectionTitle: {
    fontSize: 24,
    fontWeight: "600",
  },
  sectionDescription: {
    marginTop: 8,
    fontSize: 18,
    fontWeight: "400",
  },
  highlight: {
    fontWeight: "700",
  },
  footer: {
    fontSize: 12,
    fontWeight: "600",
    padding: 4,
    paddingRight: 12,
    textAlign: "right",
  },
});

export default App;
