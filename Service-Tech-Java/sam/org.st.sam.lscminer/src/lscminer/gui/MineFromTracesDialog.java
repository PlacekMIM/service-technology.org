/*
 * MineFromTracesDialog.java
 *
 * Created on Oct 3, 2010, 12:20:52 PM
 */

package lscminer.gui;

import javax.swing.*;

import org.st.sam.mine.MineBranchingLSC;

import java.io.*;
import java.text.SimpleDateFormat;
import java.util.*;
import lscminer.Main;
import lscminer.datastructure.*;
import lscminer.utilities.*;
import lscminer.miner.*;

/**
 *
 * @author nacuong
 */
public class MineFromTracesDialog extends javax.swing.JDialog {

    public boolean mineStatus;
    public String mineProjectName;
    public String mineProjectLocation;
    public String mineTaskName;
    public int mineSupport;
    public double mineConfidence;
    public double mineDensity;
    public String mineType;
    public String mineTime;
    public double mineRuntime;
    public ArrayList<LSC> mineLSC;

    /** Creates new form MineFromTracesDialog */
    public MineFromTracesDialog(java.awt.Frame parent, boolean modal) {
        super(parent, modal);

        mineStatus = false;
        mineLSC = new ArrayList<LSC>();
        mineProjectName = "";
        mineProjectLocation = "";
        mineTaskName = "";
        mineSupport = 0;
        mineConfidence = 0;
        mineDensity = 0;
        mineType = "mine from traces";

        initComponents();
        targetFolderTextField.setText(Main.getProjLoc());
    }

    /** This method is called from within the constructor to
     * initialize the form.
     * WARNING: Do NOT modify this code. The content of this method is
     * always regenerated by the Form Editor.
     */
    @SuppressWarnings("unchecked")
    // <editor-fold defaultstate="collapsed" desc="Generated Code">//GEN-BEGIN:initComponents
    private void initComponents() {

        newProjectPanel = new javax.swing.JPanel();
        projectNameTextField = new javax.swing.JTextField();
        projectNameLabel = new javax.swing.JLabel();
        targetFolderLable = new javax.swing.JLabel();
        targetFolderTextField = new javax.swing.JTextField();
        targetFolderBrowseButton = new javax.swing.JButton();
        taskNameLabel = new javax.swing.JLabel();
        taskNameTextField = new javax.swing.JTextField();
        jPanel1 = new javax.swing.JPanel();
        inputTraceLabel = new javax.swing.JLabel();
        inputTraceTextField = new javax.swing.JTextField();
        inputTraceBrowseButton = new javax.swing.JButton();
        miningParameterPanel = new javax.swing.JPanel();
        minSupportLabel = new javax.swing.JLabel();
        minSupportTextField = new javax.swing.JTextField();
        minConfidenceLabel = new javax.swing.JLabel();
        minConfidenceTextField = new javax.swing.JTextField();
        minDensityLabel = new javax.swing.JLabel();
        minDensityTextField = new javax.swing.JTextField();
        cancelButton = new javax.swing.JButton();
        startMiningButton = new javax.swing.JButton();
        advancedSettingsButton = new javax.swing.JButton();

        setDefaultCloseOperation(javax.swing.WindowConstants.DISPOSE_ON_CLOSE);
        setTitle("Mine from Traces Dialog");
        setIconImage(null);
        setLocationByPlatform(true);
        setResizable(false);

        newProjectPanel.setBorder(javax.swing.BorderFactory.createTitledBorder(null, "   Create New Project    ", javax.swing.border.TitledBorder.DEFAULT_JUSTIFICATION, javax.swing.border.TitledBorder.DEFAULT_POSITION, new java.awt.Font("Tahoma", 1, 12))); // NOI18N

        projectNameTextField.setFont(new java.awt.Font("Tahoma", 0, 12));
        projectNameTextField.setToolTipText("Name of the project that will be created for this session, i.e. jeti for mining Jeti chat server");
        projectNameTextField.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                projectNameTextFieldActionPerformed(evt);
            }
        });

        projectNameLabel.setFont(new java.awt.Font("Tahoma", 0, 12));
        projectNameLabel.setText("Project Name :");

        targetFolderLable.setFont(new java.awt.Font("Tahoma", 0, 12));
        targetFolderLable.setText("Target Folder :");

        targetFolderTextField.setFont(new java.awt.Font("Tahoma", 0, 12));
        targetFolderTextField.setToolTipText("The target folder where the project will be created");
        targetFolderTextField.setEnabled(false);
        targetFolderTextField.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                targetFolderTextFieldActionPerformed(evt);
            }
        });

        targetFolderBrowseButton.setFont(new java.awt.Font("Tahoma", 0, 12));
        targetFolderBrowseButton.setText("Browse");
        targetFolderBrowseButton.setFocusable(false);
        targetFolderBrowseButton.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                targetFolderBrowseButtonActionPerformed(evt);
            }
        });

        taskNameLabel.setFont(new java.awt.Font("Tahoma", 0, 12));
        taskNameLabel.setText("Task Name :");

        taskNameTextField.setFont(new java.awt.Font("Tahoma", 0, 12));
        taskNameTextField.setToolTipText("The task name for the set of input traces; i.e. mineStartChatScenario");
        taskNameTextField.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                taskNameTextFieldActionPerformed(evt);
            }
        });

        javax.swing.GroupLayout newProjectPanelLayout = new javax.swing.GroupLayout(newProjectPanel);
        newProjectPanel.setLayout(newProjectPanelLayout);
        newProjectPanelLayout.setHorizontalGroup(
            newProjectPanelLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(newProjectPanelLayout.createSequentialGroup()
                .addGap(55, 55, 55)
                .addGroup(newProjectPanelLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.TRAILING)
                    .addComponent(taskNameLabel)
                    .addGroup(newProjectPanelLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                        .addComponent(targetFolderLable)
                        .addComponent(projectNameLabel)))
                .addGap(39, 39, 39)
                .addGroup(newProjectPanelLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addGroup(newProjectPanelLayout.createSequentialGroup()
                        .addGroup(newProjectPanelLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING, false)
                            .addComponent(projectNameTextField)
                            .addComponent(targetFolderTextField, javax.swing.GroupLayout.DEFAULT_SIZE, 324, Short.MAX_VALUE))
                        .addGap(18, 18, 18)
                        .addComponent(targetFolderBrowseButton))
                    .addComponent(taskNameTextField, javax.swing.GroupLayout.PREFERRED_SIZE, 324, javax.swing.GroupLayout.PREFERRED_SIZE))
                .addContainerGap(52, Short.MAX_VALUE))
        );
        newProjectPanelLayout.setVerticalGroup(
            newProjectPanelLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(newProjectPanelLayout.createSequentialGroup()
                .addGap(11, 11, 11)
                .addGroup(newProjectPanelLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                    .addComponent(projectNameLabel)
                    .addComponent(projectNameTextField, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE))
                .addGap(18, 18, 18)
                .addGroup(newProjectPanelLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                    .addComponent(targetFolderLable)
                    .addComponent(targetFolderTextField, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addComponent(targetFolderBrowseButton, javax.swing.GroupLayout.PREFERRED_SIZE, 23, javax.swing.GroupLayout.PREFERRED_SIZE))
                .addGap(18, 18, 18)
                .addGroup(newProjectPanelLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                    .addComponent(taskNameLabel)
                    .addComponent(taskNameTextField, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE))
                .addContainerGap(javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE))
        );

        jPanel1.setBorder(javax.swing.BorderFactory.createTitledBorder(null, "   Enter Input Trace   ", javax.swing.border.TitledBorder.DEFAULT_JUSTIFICATION, javax.swing.border.TitledBorder.DEFAULT_POSITION, new java.awt.Font("Tahoma", 1, 12))); // NOI18N

        inputTraceLabel.setFont(new java.awt.Font("Tahoma", 0, 12));
        inputTraceLabel.setText("Input Trace :");

        inputTraceTextField.setFont(new java.awt.Font("Tahoma", 0, 12));
        inputTraceTextField.setToolTipText("The set of input traces for the particular task");
        inputTraceTextField.setEnabled(false);
        inputTraceTextField.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                inputTraceTextFieldActionPerformed(evt);
            }
        });

        inputTraceBrowseButton.setFont(new java.awt.Font("Tahoma", 0, 12));
        inputTraceBrowseButton.setText("Browse");
        inputTraceBrowseButton.setFocusable(false);
        inputTraceBrowseButton.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                inputTraceBrowseButtonActionPerformed(evt);
            }
        });

        javax.swing.GroupLayout jPanel1Layout = new javax.swing.GroupLayout(jPanel1);
        jPanel1.setLayout(jPanel1Layout);
        jPanel1Layout.setHorizontalGroup(
            jPanel1Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(jPanel1Layout.createSequentialGroup()
                .addGap(60, 60, 60)
                .addComponent(inputTraceLabel)
                .addGap(41, 41, 41)
                .addComponent(inputTraceTextField, javax.swing.GroupLayout.PREFERRED_SIZE, 324, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addGap(18, 18, 18)
                .addComponent(inputTraceBrowseButton)
                .addContainerGap(54, Short.MAX_VALUE))
        );
        jPanel1Layout.setVerticalGroup(
            jPanel1Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(jPanel1Layout.createSequentialGroup()
                .addContainerGap()
                .addGroup(jPanel1Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                    .addComponent(inputTraceLabel)
                    .addComponent(inputTraceTextField, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addComponent(inputTraceBrowseButton, javax.swing.GroupLayout.PREFERRED_SIZE, 23, javax.swing.GroupLayout.PREFERRED_SIZE))
                .addContainerGap(21, Short.MAX_VALUE))
        );

        miningParameterPanel.setBorder(javax.swing.BorderFactory.createTitledBorder(null, "   Enter Mining Parameter   ", javax.swing.border.TitledBorder.DEFAULT_JUSTIFICATION, javax.swing.border.TitledBorder.DEFAULT_POSITION, new java.awt.Font("Tahoma", 1, 12))); // NOI18N

        minSupportLabel.setFont(new java.awt.Font("Tahoma", 0, 12));
        minSupportLabel.setText("Minimum Support :");

        minSupportTextField.setFont(new java.awt.Font("Tahoma", 0, 12));
        minSupportTextField.setToolTipText("Support is the number of occurrences of an LSC that appear in the set of traces");
        minSupportTextField.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                minSupportTextFieldActionPerformed(evt);
            }
        });

        minConfidenceLabel.setFont(new java.awt.Font("Tahoma", 0, 12));
        minConfidenceLabel.setText("Minimum Confidence :");

        minConfidenceTextField.setFont(new java.awt.Font("Tahoma", 0, 12));
        minConfidenceTextField.setToolTipText("Confidence is the likelihood that the LSC will be satisfied in the set of traces; set to 1 to mine 100% correctness LSC");
        minConfidenceTextField.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                minConfidenceTextFieldActionPerformed(evt);
            }
        });

        minDensityLabel.setFont(new java.awt.Font("Tahoma", 0, 12));
        minDensityLabel.setText("Minimum Density :");

        minDensityTextField.setFont(new java.awt.Font("Tahoma", 0, 12));
        minDensityTextField.setToolTipText("Density is the ratio of distinct events over all events in the LSC, set to 1 to mine LSC of distinct events");
        minDensityTextField.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                minDensityTextFieldActionPerformed(evt);
            }
        });

        cancelButton.setFont(new java.awt.Font("Tahoma", 0, 14));
        cancelButton.setText("Cancel");
        cancelButton.setFocusPainted(false);
        cancelButton.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                cancelButtonActionPerformed(evt);
            }
        });

        startMiningButton.setFont(new java.awt.Font("Tahoma", 0, 14));
        startMiningButton.setText("Start Mining");
        startMiningButton.setFocusPainted(false);
        startMiningButton.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                startMiningButtonActionPerformed(evt);
            }
        });

        advancedSettingsButton.setFont(new java.awt.Font("Tahoma", 0, 14));
        advancedSettingsButton.setText("Advanced Settings");
        advancedSettingsButton.setFocusPainted(false);

        javax.swing.GroupLayout miningParameterPanelLayout = new javax.swing.GroupLayout(miningParameterPanel);
        miningParameterPanel.setLayout(miningParameterPanelLayout);
        miningParameterPanelLayout.setHorizontalGroup(
            miningParameterPanelLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(miningParameterPanelLayout.createSequentialGroup()
                .addContainerGap()
                .addGroup(miningParameterPanelLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.TRAILING)
                    .addComponent(minSupportLabel)
                    .addComponent(minConfidenceLabel)
                    .addComponent(minDensityLabel))
                .addGap(46, 46, 46)
                .addGroup(miningParameterPanelLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addComponent(minConfidenceTextField, javax.swing.GroupLayout.PREFERRED_SIZE, 324, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addComponent(minSupportTextField, javax.swing.GroupLayout.PREFERRED_SIZE, 324, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addGroup(miningParameterPanelLayout.createSequentialGroup()
                        .addGroup(miningParameterPanelLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.TRAILING)
                            .addComponent(minDensityTextField, javax.swing.GroupLayout.PREFERRED_SIZE, 324, javax.swing.GroupLayout.PREFERRED_SIZE)
                            .addGroup(miningParameterPanelLayout.createSequentialGroup()
                                .addComponent(advancedSettingsButton)
                                .addGap(18, 18, 18)
                                .addComponent(startMiningButton)))
                        .addGap(18, 18, 18)
                        .addComponent(cancelButton)))
                .addContainerGap(53, Short.MAX_VALUE))
        );
        miningParameterPanelLayout.setVerticalGroup(
            miningParameterPanelLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(miningParameterPanelLayout.createSequentialGroup()
                .addContainerGap()
                .addGroup(miningParameterPanelLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                    .addComponent(minSupportLabel)
                    .addComponent(minSupportTextField, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE))
                .addGap(18, 18, 18)
                .addGroup(miningParameterPanelLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                    .addComponent(minConfidenceLabel)
                    .addComponent(minConfidenceTextField, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE))
                .addGap(18, 18, 18)
                .addGroup(miningParameterPanelLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addComponent(minDensityLabel)
                    .addComponent(minDensityTextField, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE))
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED, 34, Short.MAX_VALUE)
                .addGroup(miningParameterPanelLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                    .addComponent(startMiningButton, javax.swing.GroupLayout.PREFERRED_SIZE, 39, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addComponent(cancelButton, javax.swing.GroupLayout.PREFERRED_SIZE, 39, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addComponent(advancedSettingsButton, javax.swing.GroupLayout.PREFERRED_SIZE, 38, javax.swing.GroupLayout.PREFERRED_SIZE))
                .addContainerGap())
        );

        javax.swing.GroupLayout layout = new javax.swing.GroupLayout(getContentPane());
        getContentPane().setLayout(layout);
        layout.setHorizontalGroup(
            layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(layout.createSequentialGroup()
                .addContainerGap()
                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addComponent(newProjectPanel, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                    .addComponent(jPanel1, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                    .addComponent(miningParameterPanel, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE))
                .addContainerGap())
        );
        layout.setVerticalGroup(
            layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(layout.createSequentialGroup()
                .addContainerGap()
                .addComponent(newProjectPanel, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addGap(18, 18, 18)
                .addComponent(jPanel1, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.UNRELATED)
                .addComponent(miningParameterPanel, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addContainerGap(javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE))
        );

        pack();
    }// </editor-fold>//GEN-END:initComponents

    private void projectNameTextFieldActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_projectNameTextFieldActionPerformed
        // TODO add your handling code here:
}//GEN-LAST:event_projectNameTextFieldActionPerformed

    private void targetFolderTextFieldActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_targetFolderTextFieldActionPerformed
        // TODO add your handling code here:
}//GEN-LAST:event_targetFolderTextFieldActionPerformed

    private void targetFolderBrowseButtonActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_targetFolderBrowseButtonActionPerformed
        JFileChooser targetFolderChooser = new JFileChooser();
        targetFolderChooser.setCurrentDirectory(new File(targetFolderTextField.getText()));
        targetFolderChooser.setDialogTitle("Select a Target Folder");
        targetFolderChooser.setApproveButtonText("Select");
        targetFolderChooser.setApproveButtonToolTipText("Select target folder");
        targetFolderChooser.setFileSelectionMode(JFileChooser.DIRECTORIES_ONLY);
        targetFolderChooser.setAcceptAllFileFilterUsed(false);
        if (targetFolderChooser.showOpenDialog(this) == JFileChooser.APPROVE_OPTION) {
            targetFolderTextField.setText(targetFolderChooser.getSelectedFile().getPath());
        }
}//GEN-LAST:event_targetFolderBrowseButtonActionPerformed

    private void taskNameTextFieldActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_taskNameTextFieldActionPerformed
        // TODO add your handling code here:
}//GEN-LAST:event_taskNameTextFieldActionPerformed

    private void inputTraceTextFieldActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_inputTraceTextFieldActionPerformed
        // TODO add your handling code here:
}//GEN-LAST:event_inputTraceTextFieldActionPerformed

    private void inputTraceBrowseButtonActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_inputTraceBrowseButtonActionPerformed
        JFileChooser inputTraceChooser = new JFileChooser();
        inputTraceChooser.setCurrentDirectory(new File(System.getProperty("user.dir")));
        inputTraceChooser.setDialogTitle("Select Input Traces");
        inputTraceChooser.setApproveButtonText("Select");
        inputTraceChooser.setApproveButtonToolTipText("Select input traces");
        inputTraceChooser.setFileSelectionMode(JFileChooser.FILES_ONLY);
        inputTraceChooser.setMultiSelectionEnabled(true);
        inputTraceChooser.setAcceptAllFileFilterUsed(true);
        inputTraceChooser.addChoosableFileFilter(new TraceFilter());

        if (inputTraceChooser.showOpenDialog(this) == JFileChooser.APPROVE_OPTION) {
            File[] traceFiles = inputTraceChooser.getSelectedFiles();
            String traces = "";
            for (int inx = 0; inx < traceFiles.length; inx++){
                traces += traceFiles[inx].getPath();
                if (inx != traceFiles.length-1){
                    traces += ";";
                }
            }
            inputTraceTextField.setText(traces);
        }
}//GEN-LAST:event_inputTraceBrowseButtonActionPerformed

    private void minSupportTextFieldActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_minSupportTextFieldActionPerformed
        // TODO add your handling code here:
}//GEN-LAST:event_minSupportTextFieldActionPerformed

    private void minConfidenceTextFieldActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_minConfidenceTextFieldActionPerformed
        // TODO add your handling code here:
}//GEN-LAST:event_minConfidenceTextFieldActionPerformed

    private void minDensityTextFieldActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_minDensityTextFieldActionPerformed
        // TODO add your handling code here:
}//GEN-LAST:event_minDensityTextFieldActionPerformed

    private void cancelButtonActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_cancelButtonActionPerformed
        this.dispose();
}//GEN-LAST:event_cancelButtonActionPerformed

    private void startMiningButtonActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_startMiningButtonActionPerformed
        String projectName = "";
        String targetFolder = "";

        String taskName = "";
        String[] inputTraces = null;
        int minSupport = 2;
        double minConfidence = 1;
        double minDensity = 1;

        boolean createProjectError = false;
        boolean miningParameterError = false;

        // <editor-fold desc="step 0: create a new project">
        projectName = projectNameTextField.getText();
        /* project name cannot be empty */
        if (projectName.equals("")){
            JOptionPane.showMessageDialog(this, "Project name cannot be empty!", "Error Message", JOptionPane.ERROR_MESSAGE);
            projectNameTextField.requestFocusInWindow();
            createProjectError = true;
        } else {
            targetFolder = targetFolderTextField.getText();
            targetFolder = targetFolder.replace('\\', '/');
            taskName = taskNameTextField.getText();
            File taskFolder = new File(targetFolder + "/" + projectName + "/" + taskName);
            /* project must be successfully created */
            if (taskFolder.exists() && taskFolder.isDirectory()) {
                System.out.println(targetFolder + "/" + projectName + "/" + taskName);
                JOptionPane.showMessageDialog(this, "The project already exists! Please create another project.", "Error Message", JOptionPane.ERROR_MESSAGE);
                projectNameTextField.requestFocusInWindow();
                createProjectError = true;
            }
        }
        // </editor-fold>

        // <editor-fold desc="step 1: validate mining parameter">
        if (!createProjectError){
            taskName = taskNameTextField.getText();
            /* task name cannot be empty */
            if (taskName.equals("")){
                miningParameterError = true;
                JOptionPane.showMessageDialog(this, "Task name cannot be empty!", "Error Message", JOptionPane.ERROR_MESSAGE);
                taskNameTextField.requestFocusInWindow();
            } else {
                /* input traces cannot be empty */
                if (inputTraceTextField.getText().equals("")){
                    miningParameterError = true;
                    JOptionPane.showMessageDialog(this, "Please input at least one trace!", "Error Message", JOptionPane.ERROR_MESSAGE);
                    inputTraceTextField.requestFocusInWindow();
                } else {
                    inputTraces = inputTraceTextField.getText().split(";");
                    /* minimum support must be integer greater than 2 */
                    try {
                        minSupport = Integer.parseInt(minSupportTextField.getText().trim());
                        if (minSupport < 2){
                            miningParameterError = true;
                            JOptionPane.showMessageDialog(this, "Minimum support must be a positive integer greater than or equal to 2!", "Error Message", JOptionPane.ERROR_MESSAGE);
                            minSupportTextField.requestFocusInWindow();
                        } else {
                            /* minimum confidence must be a double between 0 and 1 */
                            try {
                                minConfidence = Double.parseDouble(minConfidenceTextField.getText().trim());
                                if (minConfidence > 1.0 || minConfidence < 0.0){
                                    miningParameterError = true;
                                    JOptionPane.showMessageDialog(this, "Minimum confidence must be a double between 0 and 1!", "Error Message", JOptionPane.ERROR_MESSAGE);
                                    minConfidenceTextField.requestFocusInWindow();
                                } else {
                                    try {
                                        minDensity = Double.parseDouble(minDensityTextField.getText().trim());
                                        if (minDensity > 1.0 || minDensity < 0.0){
                                            miningParameterError = true;
                                            JOptionPane.showMessageDialog(this, "Minimum density must be a double between 0 and 1!", "Error Message", JOptionPane.ERROR_MESSAGE);
                                            minDensityTextField.requestFocusInWindow();
                                        }
                                    } catch (Exception e) {
                                        miningParameterError = true;
                                        JOptionPane.showMessageDialog(this, "Minimum density must be a double between 0 and 1!", "Error Message", JOptionPane.ERROR_MESSAGE);
                                        minDensityTextField.requestFocusInWindow();
                                    }
                                }
                            } catch (Exception e){
                                miningParameterError = true;
                                JOptionPane.showMessageDialog(this, "Minimum confidence must be an double between 0 and 1!", "Error Message", JOptionPane.ERROR_MESSAGE);
                                minConfidenceTextField.requestFocusInWindow();
                            }
                        }
                    } catch (Exception e){
                        miningParameterError = true;
                        JOptionPane.showMessageDialog(this, "Minimum support must be a positive integer greater than or equal to 2!", "Error Message", JOptionPane.ERROR_MESSAGE);
                        minSupportTextField.requestFocusInWindow();
                    }
                }
            }
        }
        // </editor-fold>

        // <editor-fold desc="step 2: start mining process">
        boolean mineFail = true;
        if (!createProjectError && !miningParameterError) {
            // <editor-fold desc="start mining">
          {
                // <editor-fold desc="create folder for storing mining results">
                /* create a folder to store mining result */
                String projectPath = targetFolder + "/" + projectName;
                File project = new File(projectPath);
                project.mkdir();
                /* create a folder for this task */
                String taskPath = projectPath + "/" + taskName;
                File task = new File(taskPath);
                task.mkdir();
                /* create folder to store traces */
                File tracesFile = new File(taskPath + "/traces");
                tracesFile.mkdir();
                File patternsFile = new File(taskPath + "/patterns.txt");
                File ltlFile = new File(taskPath + "/ltls.txt");
                File lscFile = new File(taskPath + "/lscs.txt");
                File logFile = new File(taskPath + "/log.txt");
                try {
                    /* create patterns file to store pattern results */
                    patternsFile.createNewFile();
                    /* create ltls file to store ltl results */
                    ltlFile.createNewFile();
                    /* create lscs file to store lsc results */
                    lscFile.createNewFile();
                    /* create log file to store mining parametric information */
                    logFile.createNewFile();
                } catch (IOException e) {
                }
                // </editor-fold>

                // LSCMiner lscMiner = new LSCMiner();
                MineBranchingLSC lscMiner = new MineBranchingLSC();
                long before = System.currentTimeMillis();
                try {
                  lscMiner.mineLSCs(inputTraceTextField.getText(), minSupport, minConfidence);
                } catch (IOException e) {
                  System.out.println(e);
                }
                long after = System.currentTimeMillis();
                this.mineRuntime = ((double)(after-before))/1000.0;
                this.mineLSC = lscMiner.getLSCs();
                //this.mineDataSet = dataSet;
                try {
                    /* record mined dataset */
                  /*
                    for (int traceInx = 0; traceInx < mineDataSet.length; traceInx++){
                      LSCEvent[] trace = mineDataSet[traceInx];
                      File traceFile = new File(targetFolder + "/" + projectName + "/" + taskName + "/traces/trace" + traceInx + ".txt");
                      traceFile.createNewFile();
                      PrintWriter tracePw = new PrintWriter(traceFile.getAbsolutePath());
                      for (LSCEvent event: trace){
                        tracePw.println(event);
                      }
                      tracePw.close();
                    }
                   */
                    /* record mined lscs */
                    PrintWriter lscPw = new PrintWriter(lscFile.getAbsolutePath());
                    for (LSC lsc: this.mineLSC){
                        lscPw.println(lsc);
                        lscPw.println("--------------------------------");
                    }
                    lscPw.close();
                    /* record mine parameter */
                    mineProjectName = projectName;
                    mineProjectLocation = targetFolder + "/" + projectName;
                    mineTaskName = taskName;
                    mineSupport = minSupport;
                    mineConfidence = minConfidence;
                    mineDensity = minDensity;
                    SimpleDateFormat sdf = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
                    mineTime = sdf.format(Calendar.getInstance().getTime());
                    /* record mining parameter */
                    PrintWriter logPw = new PrintWriter(logFile.getAbsolutePath());
                    logPw.println("projectName="+this.mineProjectName);
                    logPw.println("taskName="+this.mineTaskName);
                    logPw.println("projectLocation="+this.mineProjectLocation);
                    logPw.println("support="+this.mineSupport);
                    logPw.println("confidence="+this.mineConfidence);
                    logPw.println("density="+this.mineDensity);
                    logPw.println("mineType="+this.mineType);
                    logPw.println("mineTime="+this.mineTime);
                    logPw.println("mineRuntime="+this.mineRuntime);
                    logPw.close();

                    mineFail = false;
                } catch (IOException e){
                }
            }
            // </editor-fold>
        }
        // </editor-fold>

        // <editor-fold desc="step 3: finish mining">
        if (!mineFail){
            mineStatus = true;

            /* record recently created project into history file */
            File history = new File(MainFrame.hitoryFileLocation);
            if (!history.exists()){
                try {
                    history.createNewFile();
                } catch (IOException ex) {
                    JOptionPane.showMessageDialog(this, "Cannot create history file!", "Error Message", JOptionPane.ERROR_MESSAGE);
                }
            }

            if (history.exists()){
                try {
                    PrintWriter historyPw = new PrintWriter(new FileOutputStream(history, true));
                    historyPw.println(this.mineProjectName);
                    historyPw.println(this.mineProjectLocation);
                    historyPw.close();
                } catch (FileNotFoundException ex) {
                    JOptionPane.showMessageDialog(this, "Cannot write to history file!", "Error Message", JOptionPane.ERROR_MESSAGE);
                }
            }

            this.dispose();
            JOptionPane.showMessageDialog(this, "Mining Complete!", "Plain Message", JOptionPane.PLAIN_MESSAGE);
        }
        // </editor-fold>
}//GEN-LAST:event_startMiningButtonActionPerformed

    // Variables declaration - do not modify//GEN-BEGIN:variables
    private javax.swing.JButton advancedSettingsButton;
    private javax.swing.JButton cancelButton;
    private javax.swing.JButton inputTraceBrowseButton;
    private javax.swing.JLabel inputTraceLabel;
    private javax.swing.JTextField inputTraceTextField;
    private javax.swing.JPanel jPanel1;
    private javax.swing.JLabel minConfidenceLabel;
    private javax.swing.JTextField minConfidenceTextField;
    private javax.swing.JLabel minDensityLabel;
    private javax.swing.JTextField minDensityTextField;
    private javax.swing.JLabel minSupportLabel;
    private javax.swing.JTextField minSupportTextField;
    private javax.swing.JPanel miningParameterPanel;
    private javax.swing.JPanel newProjectPanel;
    private javax.swing.JLabel projectNameLabel;
    private javax.swing.JTextField projectNameTextField;
    private javax.swing.JButton startMiningButton;
    private javax.swing.JButton targetFolderBrowseButton;
    private javax.swing.JLabel targetFolderLable;
    private javax.swing.JTextField targetFolderTextField;
    private javax.swing.JLabel taskNameLabel;
    private javax.swing.JTextField taskNameTextField;
    // End of variables declaration//GEN-END:variables

}